#!/bin/usr/python

from __future__ import print_function
import clingo
import sys
from collections import namedtuple
from time import clock

# defines
STR_UNSAT = "error: input program is UNSAT"
GROUNDING_ERROR = "error: invalid grounding steps"
INIT = "init"
PRIMED_EXTERNAL = -1
TRUE  = -2
FALSE = -3

log_level = 0
def log(*args):
    if log_level == 1:
        print(*args)


#
# Syntax Restriction:
#   - primed atoms are not allowed in the heads
#
# Semantics:
#   - the transition is defined by the stable models of the program
#   - the possible initial states are defined by the set of primed externals
#   - normal (not primed) externals work as usual
#
# Extensions (made by the controller object):
#   - init/1 defines the initial situation
#   - external last/0 is set to true only at the last step
#


#
# example program
#

base = """
% background
dom(1..10000).
{a(X)} :- dom(X).
{b(X)} :- dom(X).
:- a(X), b(X).
:- a(X), not b(X).
:- not a(X), b(X).
a :- a(X).
fluent(loaded) :- not a.
fluent(alive)  :- not a.
value(true)    :- not a.
value(false)   :- not a.
action(load)   :- not a.
action(shoot)  :- not a.
#show fluent/1.
#show a/0.
na :- not a. #show na/0.
#show a/1. #show b/1.

% transition generation
holds(loaded,true) :- occ(load).
holds(alive,false) :- occ(shoot), holds'(loaded,true).
holds(F,V) :- holds'(F,V), not holds(F,VV) : value(VV), VV != V.
{ occ(A) : action(A) } 1.
:- holds(alive,true), last.
#show occ/1.

% state generation
#external holds'(F,V) : fluent(F), value(V).

% other externals
#external last.

% init
init(holds(loaded,false)).
init(holds(alive,true)).

% extension: always one wait action
action(wait).
done(wait) :- occ(wait).
done(wait) :- done'(wait).
:- last, not done(wait).
#external done'(wait).

"""


class DLPGenerator:

    def __init__(self, files = [], adds = [], parts = [], options = []):
        # input
        self.files = files
        self.adds = adds
        self.parts = parts
        self.options = options
        # output
        self.offset = 0
        self.rules = []
        self.weight_rules = []
        self.primed_externals = {}
        self.normal_externals = {}
        self.output = []
        self.output_facts = []
        self.init = []
        # rest
        self.ctl = None
        self.next = {}
        self.mapping = []

    def run(self):
        # preliminaries
        time0 = clock()
        ctl = self.ctl = clingo.Control(self.options)
        ctl.register_observer(self)
        for i in self.files:
            ctl.load(i)
        for i in self.adds:
            ctl.add(i[0], i[1], i[2])
        ctl.ground(self.parts)
        # analyze
        self.set_externals()
        log("preliminaries:\t {:.2f}s".format(clock()-time0))
        time0 = clock()
        self.simplify()
        log("simplify:\t {:.2f}s".format(clock()-time0))
        time0 = clock()
        self.set_next()
        self.set_mapping()
        self.map_rules()
        self.map_weight_rules()
        self.handle_externals()
        self.set_output()
        self.set_init()
        log("rest:\t {:.2f}s".format(clock()-time0))
        # return
        return DynamicLogicProgram(
            self.offset, self.rules, self.weight_rules,
            self.primed_externals, self.normal_externals,
            self.output, self.output_facts, self.init
        )

    def set_externals(self):
        for x in self.ctl.symbolic_atoms:
            if x.is_external:
                self.ctl.assign_external(x.symbol, None)
                if len(x.symbol.name) and x.symbol.name[-1]=="'":
                    self.primed_externals[x.symbol] = x.literal
                else:
                    self.normal_externals[x.symbol] = x.literal

    def simplify(self):
        pass

    def get_next_literal(self, symbol, default):
        try:
            next_symbol = clingo.Function(symbol.name[:-1], symbol.arguments)
            return self.ctl.symbolic_atoms[next_symbol].literal
        except Exception:
            return default

    def set_next(self):
        self.next = {literal : self.get_next_literal(symbol, literal)
                     for symbol, literal in self.primed_externals.items() }

    def set_mapping(self):
        self.mapping = [0] * (self.offset + 1) # TODO: check if offset works
        for i in range(1, self.offset + 1):
            self.mapping[i] = self.next.get(i, i + self.offset)

    def map_rules(self):
        for rule in self.rules:
            for i, atom in enumerate(rule[1]):
                rule[1][i] = self.mapping[atom]
            for i, atom in enumerate(rule[2]):
                if atom>0:
                    rule[2][i] = self.mapping[atom]
                else:
                    rule[2][i] = -self.mapping[-atom]

    def map_weight_rules(self):
        for rule in self.weight_rules:
            for i, atom in enumerate(rule[1]):
                rule[1][i] = self.mapping[atom]
            for i, atom in enumerate(rule[3]):
                if atom[0]>0:
                    rule[3][i] = (self.mapping[atom[0]], atom[1])
                else:
                    rule[3][i] = (-self.mapping[-atom[0]], atom[1])

    def handle_externals(self):
        for symbol, literal in self.primed_externals.items():
            self.primed_externals[symbol] = self.mapping[literal]
        for symbol, literal in self.normal_externals.items():
            self.normal_externals[symbol] = self.mapping[literal] - self.offset
            self.rules.append((True, [self.mapping[literal]], []))

    def set_output(self):
        # gather output
        with self.ctl.solve(yield_=True) as handle:
            for m in handle:
                break
            if handle.get().unsatisfiable:
                raise Exception(STR_UNSAT)
        # map
        idx = 0
        for atom, symbol in self.output:
            if atom == 0:
                self.output_facts.append(symbol)
                continue
            mapped_atom = self.mapping[atom]
            if mapped_atom > self.offset:
                mapped_atom -= self.offset
            elif mapped_atom:                              # primed external
                symbol = clingo.Function(symbol.name[:-1], symbol.arguments)
            self.output[idx] = (mapped_atom, symbol)
            idx += 1
        self.output = self.output[0:idx]

    def set_init(self):
        self.init = [self.mapping[
                         self.ctl.symbolic_atoms[
                             clingo.Function(i.symbol.arguments[0].name+"'",
                                             i.symbol.arguments[0].arguments
                                            )
                         ].literal
                     ] for i in self.ctl.symbolic_atoms if i.symbol.name == INIT
        ]

    #
    # observer
    #

    def update_offset(self, _list):
        for i in _list:
            if abs(i)>self.offset:
                self.offset = abs(i)

    def rule(self, choice, head, body):
        self.update_offset(head)
        self.update_offset(body)
        self.rules.append((choice, head, body))

    def weight_rule(self, choice, head, lower_bound, body):
        self.update_offset(head)
        self.update_offset([x for x,y in body])
        self.weight_rules.append((choice, head, lower_bound, body))

    def external(self, atom, value):
        self.update_offset([atom])

    def output_atom(self, symbol, atom):
        self.output.append((atom, symbol))

    #
    # __str__
    #

    def __str__(self):
        out = ""
        out += "\nOFFSET\n" + str(self.offset)
        out += "\nMAPPING\n" + "\n".join(
            ["{}:{}".format(i, item)
             for i, item in enumerate(self.mapping)]
        )
        out += "\nRULES\n" + "\n".join(
            ["{}:{}:{}".format(
                i[0], i[1], i[2]
            ) for i in self.rules if i is not None]
        )
        out += "\nWEIGHT RULES\n" + "\n".join(
            ["{}:{}:{}:{}".format(
                i[0], i[1], i[2], i[3]
            ) for i in self.weight_rules if i is not None]
        )
        out += "\nPRIMED EXTERNALS\n" + "\n".join(
            ["{}:{}".format(x, y) for x, y in sorted(
                self.primed_externals.items()
            )]
        )
        out += "\nNORMAL EXTERNALS\n" + "\n".join(
            ["{}:{}".format(x, y) for x, y in sorted(
                self.normal_externals.items()
            )]
        )
        out += "\nOUTPUT\n" + "\n".join(
            ["{}:{}".format(x, y) for x, y in sorted(self.output)]
        )
        out += "\nOUTPUT FACTS\n" + "\n".join(
            ["{}".format(x) for x in sorted(self.output_facts)]
        )
        out += "\nINIT\n" + "\n".join(
            ["{}".format(x) for x in sorted(self.init)]
        )
        return out


SAtom  = namedtuple('SAtom', 
                    [ 'heads',  'bodypos',  'bodyneg',
                     'wheads', 'wbodypos', 'wbodyneg'])
# heads, bodypos and bodyneg are lists of rule positions,
# wheads is a list of weight_wrule positions,
# and wbodypos and wbodyneg are tuples (weight, weight_rule position)

class DLPGeneratorSimplifier(DLPGenerator):

    def __init__(self, files = [], adds = [], parts = [], options = []):
        DLPGenerator.__init__(self, files, adds, parts, options)
        self.satoms = []
        self.true = []
        self.false = []
        self.cautious = set()
        self.add_facts = []
        self.add_constraints = []

    def simplify(self):
        self.mapping = [None]*len(self.satoms)
        self.offset = len(self.satoms)
        time0 = clock()
        self.false = self.get_consequences("brave", False)
        log("\tbrave:\t\t {:.2f}s".format(clock()-time0))
        time0 = clock()
        self.cautious = set(self.get_consequences("cautious", True))
        log("\tcautious:\t {:.2f}s".format(clock()-time0))
        time0 = clock()
        self.fitting()
        log("\tfitting:\t {:.2f}s".format(clock()-time0))

    def get_consequences(self, opt, true):
        self.ctl.configuration.solve.enum_mode = opt
        with self.ctl.solve(yield_=True) as handle:
            last = None
            for m in handle:
                last = m
            if last is None:
                raise Exception(STR_UNSAT)
            if true:
                symbols = last.symbols(shown=True)
            else:
                symbols = last.symbols(shown=True, complement=True)
            return [self.ctl.symbolic_atoms[x].literal for x in symbols]

    def remove_rule_from_heads(self, rule, atom, weight=False):
        satom = self.satoms[atom]
        if weight:
            satom.wheads.remove(rule)
        else:
            satom.heads.remove(rule)
        if not satom.heads and not satom.wheads:
            self.false.append(atom)

    def fitting(self):
        while True:
            # preprocess
            if len(self.true):
                atom = self.true[0]
                self.true = self.true[1:]
                true = True
                signed = atom
                self.mapping[atom] = TRUE
                self.cautious.discard(atom)
                self.offset -= 1
            elif len(self.false):
                atom = self.false[0]
                self.false = self.false[1:]
                true = False
                signed = -atom
                self.mapping[atom] = FALSE
                self.offset -= 1
            else:
                return
            satom = self.satoms[atom]
            if satom is None:
                continue
            if true:
                satisfied_body    = satom.bodypos
                unsatisfied_body  = satom.bodyneg
                satisfied_wbody   = satom.wbodypos
                unsatisfied_wbody = satom.wbodyneg
            else:
                satisfied_body    = satom.bodyneg
                unsatisfied_body  = satom.bodypos
                satisfied_wbody   = satom.wbodyneg
                unsatisfied_wbody = satom.wbodypos
            # heads
            for i in satom.heads:
                rule = self.rules[i]
                if rule is None:
                    continue
                if not rule[0]: # disjunction
                    if true:
                        for head in rule[1]:
                            if head != atom:
                                self.remove_rule_from_heads(i, head)
                        self.rules[i] = None
                    else:
                        rule[1].remove(atom)
                else:           # choice
                    rule[1].remove(atom)
                    if not rule[1]:
                        self.rules[i] = None
            # wheads
            for i in satom.wheads:
                rule = self.weight_rules[i]
                if rule is None:
                    continue
                if not rule[0]: # disjunction
                    if true:
                        for head in rule[1]:
                            if head != atom:
                                self.remove_rule_from_heads(i, head, True)
                        self.weight_rules[i] = None
                    else:
                        rule[1].remove(atom)
                else:            # choice
                    rule[1].remove(atom)
                    if not rule[1]:
                        self.weight_rules[i] = None
            # satisfied body
            for i in satisfied_body:
                rule = self.rules[i]
                if rule is None:
                    continue
                rule[2].remove(signed)
                if not rule[0] and len(rule[1]) == 1 and not rule[2]: # fact
                    self.true.append(rule[1].pop())
                    self.rules[i] = None
            # unsatisfied body
            for i in unsatisfied_body:
                rule = self.rules[i]
                if rule is None:
                    continue
                for head in rule[1]:
                    self.remove_rule_from_heads(i, head)
                self.rules[i] = None
            # satisfied wbody
            for w, i in satisfied_wbody:
                rule = self.weight_rules[i]
                if rule is None:
                    continue
                rule[3].remove((w,signed))
                rule[2][0] -= w
                if rule[2][0] <= 0:
                    if not rule[0] and len(rule[1]) == 1: # fact
                        self.true.append(rule[1].pop())
                    else:
                        new_rule = len(self.rules)
                        self.rules.append((rule[0], rule[1], []))
                        for head in rule[1]:
                            head.wheads.remove(i)
                            head.heads.add(new_rule)
                    self.weight_rules[i] = None
            # unsatisfied wbody
            for w, i in unsatisfied_wbody:
                rule = self.weight_rules[i]
                if rule is None:
                    continue
                rule[3].remove((w,signed))
                if sum([a for a, _ in rule[3]]) < rule[2][0]:
                    for head in rule[1]:
                        self.remove_rule_from_heads(i, head, True)
                    self.weight_rules[i] = None
            # finish
            self.satoms[atom] = None

    def set_next(self):
        for symbol, literal in self.primed_externals.items():
            self.mapping[literal] = PRIMED_EXTERNAL
            next_literal = self.get_next_literal(symbol, 0)
            self.next[literal] = next_literal
            if next_literal and self.mapping[next_literal] not in {TRUE,FALSE}:
                self.offset -= 1

    def set_mapping(self):
        number = -1           # we do self.mapping[0]=offset
        for idx, item in enumerate(self.mapping):
            if item is None:
                number += 1
                self.mapping[idx] = number + self.offset
        for symbol, literal in self.primed_externals.items():
            next_literal = self.next[literal]
            next_mapping = self.mapping[next_literal]
            if not next_literal or next_mapping == FALSE:
                number += 1
                self.mapping[literal] = number
                self.add_constraints.append(number + self.offset)
            elif next_mapping == TRUE:
                number += 1
                self.mapping[literal] = number
                self.add_constraints.append(number + self.offset)
            else:
                self.mapping[literal] = next_mapping - self.offset

    def map_rules(self):
        idx = 0
        for rule in self.rules:
            if rule is None:
                continue
            rule = self.rules[idx] = (rule[0], list(rule[1]), list(rule[2]))
            idx += 1
            for i, atom in enumerate(rule[1]):
                rule[1][i] = self.mapping[atom]
            for i, atom in enumerate(rule[2]):
                if atom > 0:
                    rule[2][i] = self.mapping[atom]
                else:
                    rule[2][i] = -self.mapping[-atom]
        self.rules = self.rules[0:idx]
        for i in self.cautious:
            self.rules.append((False, [], [-self.mapping[i]]))
        for i in self.add_facts:
            self.rules.append((False, [i], []))
        for i in self.add_constraints:
            self.rules.append((False, [], [i]))

    def map_weight_rules(self):
        idx = 0
        for rule in self.weight_rules:
            if rule is None:
                continue
            rule = self.weight_rules[idx] = (
                rule[0], list(rule[1]), rule[2][0], list(rule[3])
            )
            idx += 1
            for i, atom in enumerate(rule[1]):
                rule[1][i] = self.mapping[atom]
            for i, atom in enumerate(rule[3]):
                if atom[0] > 0:
                    rule[3][i] = (self.mapping[atom[0]], atom[1])
                else:
                    rule[3][i] = (-self.mapping[-atom[0]], atom[1])
            self.weight_rules = self.weight_rules[0:idx]

    def set_output(self):
        idx = 0
        for atom, symbol in self.output:
            mapped_atom = self.mapping[atom]
            if atom == 0 or mapped_atom == TRUE:
                self.output_facts.append(symbol)
                continue
            elif mapped_atom == FALSE:
                continue
            elif mapped_atom > self.offset:
                mapped_atom -= self.offset
            else:                                          # primed external
                symbol = clingo.Function(symbol.name[:-1], symbol.arguments)
            self.output[idx] = (mapped_atom, symbol)
            idx += 1
        self.output = self.output[0:idx]

    #
    # observer
    #

    def add_satoms(self, i, start=True):
        if i >= len(self.satoms):
            self.satoms += [None] * (i-len(self.satoms)+1)
        if start and self.satoms[i] == None:
            self.satoms[i] = SAtom(set(), [], [], set(), [], [])

    def rule(self, choice, head, body):
        # facts
        if not choice and len(head) == 1 and not len(body):
            self.add_satoms(head[0], False)
            self.true.append(head[0])
        # non facts
        else:
            rule = len(self.rules)
            for i in head:
                self.add_satoms(i)
                self.satoms[i].heads.add(rule)
            for i in body:
                self.add_satoms(abs(i))
                if i > 0:
                    self.satoms[i].bodypos.append(rule)
                else:
                    self.satoms[-i].bodyneg.append(rule)
            self.rules.append((choice, set(head), set(body)))

    def weight_rule(self, choice, head, lower_bound, body):
        # facts
        if not choice and len(head) == 1 and not len(body):
            self.add_satoms(head[0], False)
            self.true.append(head[0])
        # non facts
        else:
            wrule = len(self.weight_rules)
            for i in head:
                self.add_satoms(i)
                self.satoms[i].wheads.add(wrule)
            for i, w in body:
                self.add_satoms(abs(i))
                if i > 0:
                    self.satoms[i].wbodypos.append((w,wrule))
                else:
                    self.satoms[-i].wbodyneg.append((w,wrule))
            self.weight_rules.append(
                (choice, set(head), [lower_bound], set(body))
            )

    def external(self, atom, value):
        self.add_satoms(atom, False)


class DynamicLogicProgram:

    def __init__(self, offset, rules, weight_rules,
                 primed_externals, normal_externals,
                 output, output_facts, init):
        # init
        self.offset = offset
        self.rules = rules
        self.weight_rules = weight_rules
        self.primed_externals = primed_externals
        self.normal_externals = normal_externals
        self.output = output
        self.output_facts = output_facts
        self.init = init
        # rest
        self.ctl = None
        self.backend = None
        self.steps = 0
        self.assigned_externals = {}

    def start(self, ctl):
        self.ctl = ctl
        self.backend = ctl.backend
        #self.ctl.register_observer(self)
        for atom in self.init:
            self.backend.add_rule([atom], [], False)

    # ground(n) grounds n steps
    # ground(i,j) grounds from i to j (both included)
    def ground(self, start, end=None):
        # preprocess
        if end == None:
            end = self.steps + start
            start = self.steps + 1
        elif self.steps != start-1:
            raise Exception(GROUNDING_ERROR)
        self.steps = end
        # start
        for step in range(start, end+1):
            offset = (step-1)*self.offset
            for rule in self.rules:
                self.backend.add_rule(
                    [x+offset for x in rule[1]],
                    [x+offset for x in rule[2] if x  > 0] +
                    [x-offset for x in rule[2] if x <= 0],
                    rule[0]
                )
            for rule in self.weight_rules:
                self.backend.add_weight_rule(
                    [x+offset for x in rule[1]],
                    rule[2],
                    [(x+offset,y) for x, y in rule[3] if x  > 0] +
                    [(x-offset,y) for x, y in rule[3] if x <= 0],
                    rule[0]
                )
            for symbol in self.normal_externals.keys():
                self.assigned_externals[(step, symbol)] = -1

    def assign_external(self, step, symbol, value):
        if value is None:
            self.assigned_externals.pop((step, symbol), None)
        else:
            self.assigned_externals[(step, symbol)] = 1 if value else -1

    def release_external(self, step, symbol):
        self.assigned_externals.pop((step, symbol), None)
        self.backend.add_rule(
            [], [self.normal_externals[symbol]+(step*self.offset)], False
        )

    def get_answer(self, model, step):
        out = [((),symbol) for symbol in self.output_facts]
        for i in range(step+1):
            for atom, symbol in self.output:
                if model.is_true(atom+(i*self.offset)):
                    out.append((i, symbol))
        return out

    def get_assumptions(self):
        return [(self.normal_externals[key[1]]+(self.offset*key[0])*value)
                for key, value in self.assigned_externals.items()]

    def rule(self, choice, head, body):
        print("{}:{}:{}".format(choice, head, body))

    def weight_rule(self, choice, head, lower_bound, body):
        print("{}:{}:{}:{}".format(choice, head, lower_bound, body))


def main():
    global log_level
    log_level = 0
    time0 = clock()
    generator_class = DLPGenerator
    if len(sys.argv)>=2 and sys.argv[1] == "simple":
        generator_class = DLPGeneratorSimplifier
    generator = generator_class(
        files = [],
        adds  = [("base", [], base)],
        parts = [("base", [])],
        options = ["0"],
    )
    dynamic_lp = generator.run()
    log("generate:\t {:.2f}s".format(clock()-time0))
    time0 = clock()
    #print(generator)
    ctl = clingo.Control(["0"])
    dynamic_lp.start(ctl)
    steps = 3
    dynamic_lp.ground(steps-1)
    dynamic_lp.ground(1)
    for i in range(1,steps):
        dynamic_lp.release_external(i, clingo.parse_term("last"))
    dynamic_lp.assign_external(steps, clingo.parse_term("last"), True)
    log("ground:\t {:.2f}s".format(clock()-time0))
    time0 = clock()
    with ctl.solve(
        assumptions = dynamic_lp.get_assumptions(), 
        yield_=True
    ) as handle:
        answers = 0
        for m in handle:
            answers += 1
            print("Answer: {}".format(answers))
            answer = dynamic_lp.get_answer(m,steps)
            print(" ".join(["{}:{}".format(x,y) for x,y in answer]))
        if not answers:
            print("UNSATISFIABLE")
    log("RULES = {}, WRULES = {}".format(
        len(dynamic_lp.rules), len(dynamic_lp.weight_rules))
    )
    log("solve:\t {:.2f}s".format(clock()-time0))

if __name__ == "__main__":
    main()

