#!/bin/usr/python

import clingo
import sys
from collections import namedtuple

STR_UNSAT = "error: input program is UNSAT"
GROUNDING_ERROR = "error: invalid grounding steps"
INIT = "init"
PRIMED_EXTERNAL = -1
NORMAL_EXTERNAL = -2
TRUE  = -3
FALSE = -4

#
# Syntax Restriction:
#   - primed atoms are not allowed in the heads
#
# Semantics:
#   - the transition is defined by the stable models of the program
#   - the possible initial states are defined by the set of primed externals
#   - normal (not primed) externals work as usual
#
# Extensions:
#   - init/1 defines the initial situation
#   - external last/0 is set to true only at the last step
#

base1 = """
{a;b}.
:- a, b.
:- a, not b.

c :- not a.
e :- f, not a.
f :- e, not a.
e :- not a.
g :- 2 #sum{ 2: not a }.
"""

# example program
base = """
{a;b}. :- a, b. :- a, not b. 
b :- occ(A).
fluent(loaded) :- not a. fluent(alive) :- not a.
value(true) :- not a. value(false) :- not a.
action(load) :- not a. action(shoot) :- not a.

%*
% background
fluent(loaded). fluent(alive).
value(true). value(false).
action(load). action(shoot).
*%

% transition generation
holds(loaded,true) :- occ(load).
holds(alive,false) :- occ(shoot), holds'(loaded,true).
holds(F,V) :- holds'(F,V), not holds(F,VV) : value(VV), VV != V.
{ occ(A) : action(A) } 1.
:- holds(alive,true), last.
%:- not holds(loaded,false).
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
done(wait) :- occ(wait), not last.
done(wait) :- done'(wait).
:- last, not done(wait).
:- last, not occ(wait).
#external done'(wait).

"""

class DLPGenerator:

    def __init__(self, files = [], adds = [], parts = [], options = []):
        # input
        self.files = files
        self.adds = adds
        self.parts = parts
        self.options = options
        # rest
        self.ctl = None
        self.offset = 0
        self.rules = []
        self.weight_rules = []
        self.externals = []
        self.next = {}
        self.mapping = []
        self.primed_externals = []
        self.normal_externals = {}
        self.output = []
        self.output_facts = []
        self.init = []

    def run(self):
        # preliminaries
        ctl = self.ctl = clingo.Control(self.options)
        ctl.register_observer(self)
        for i in self.files:
            ctl.load(i)
        for i in self.adds:
            ctl.add(i[0], i[1], i[2])
        ctl.ground(self.parts)
        # analyze
        self.set_externals()
        self.simplify()
        self.set_next()
        self.set_mapping()
        self.map_rules()
        self.map_weight_rules()
        self.handle_externals()
        self.set_output()
        self.set_init()
        # return
        return DynamicLogicProgram(
            self.offset, self.rules, self.weight_rules,
            self.primed_externals, self.normal_externals, 
            self.output, self.output_facts, self.init
        )

    def simplify(self):
        pass

    def get_next_literal(self, symbol, default):
        try:
            next_symbol = clingo.Function(symbol.name[:-1], symbol.arguments)
            return self.ctl.symbolic_atoms[next_symbol].literal
        except Exception:
            return default

    def set_externals(self):
        for x in self.ctl.symbolic_atoms:
            if x.is_external:
                self.externals.append(x)
                self.ctl.assign_external(x.symbol, None)

    def set_next(self):
        self.next = {x.literal : self.get_next_literal(x.symbol, x.literal)
                     for x in self.externals if
                         len(x.symbol.name) and
                         x.symbol.name[-1] == "'"
                    }

    def set_mapping(self):
        self.mapping = [0] * (self.offset + 1)
        self.offset = self.offset # TODO: check if this works
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
        for i in self.externals:
            mapping = self.mapping[i.literal]
            if mapping <= self.offset: # init external
                self.primed_externals.append((mapping, i.symbol))
            else:                      # normal external
                self.normal_externals[i.symbol] = mapping-self.offset
                self.rules.append((True, [mapping], []))

    def set_output(self):
        # gather output
        with self.ctl.solve(yield_=True) as handle:
            for m in handle:
                break
            if handle.get().unsatisfiable:
                raise Exception(STR_UNSAT)
        # map
        for idx, (atom, symbol) in enumerate(self.output):
            mapped_atom = self.mapping[atom]
            if mapped_atom > self.offset:
                mapped_atom -= self.offset
            else: # elif mapped_atom # init external
                symbol = clingo.Function(symbol.name[:-1], symbol.arguments)
            self.output[idx] = (mapped_atom, symbol)

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

    def add_satoms(self, i, start=True):
        if i >= len(self.satoms):
            self.satoms += [None] * (i-len(self.satoms)+1)
        if start and self.satoms[i] == None:
            self.satoms[i] = SAtom(set(), [], [], set(), [], [])

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
        out += "\nINIT EXTERNALS\n" + "\n".join(
            ["{}:{}".format(x, y) for x, y in sorted(self.primed_externals)]
        )
        out += "\nNORMAL EXTERNALS\n" + "\n".join(
            ["{}:{}".format(x, y) for x, y in sorted(
                self.normal_externals.items()
            )]
        )
        out += "\nOUTPUT\n" + "\n".join(
            ["{}:{}".format(x, y) for x, y in sorted(self.output)]
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
        self.false = self.get_consequences("brave", False)
        self.cautious = set(self.get_consequences("cautious", True))
        self.fitting()

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
        for x in self.externals:
            if len(x.symbol.name) and x.symbol.name[-1]=="'":
                self.mapping[x.literal] = PRIMED_EXTERNAL
                next_literal = self.get_next_literal(x.symbol, 0)
                if next_literal and \
                   self.mapping[next_literal] not in {TRUE,FALSE}:
                    self.offset -= 1
                self.next[x.literal] = next_literal
                continue
            self.mapping[x.literal] = NORMAL_EXTERNAL

    def set_mapping(self):
        number = -1           # we do self.mapping[0]=offset
        for idx, item in enumerate(self.mapping):
            if item is None or item == NORMAL_EXTERNAL:
                number += 1
                self.mapping[idx] = number + self.offset
        for idx, item in enumerate(self.mapping):
            if item == PRIMED_EXTERNAL:
                next_literal = self.next[idx]
                next_mapping = self.mapping[next_literal]
                if not next_literal or next_mapping == FALSE:
                    number += 1
                    self.mapping[idx] = number
                    self.add_constraints.append(number + self.offset)
                elif next_mapping == TRUE:
                    number += 1
                    self.mapping[idx] = number
                    self.add_facts.append(number + self.offset)
                else:
                    self.mapping[idx] = next_mapping - self.offset

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
            self.rules.append(False, [], [-self.mapping[i]])
        for i in self.add_facts:
            self.rules.append(False, [i], [])
        for i in self.add_constraints:
            self.rules.append(False, [], [i])

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
        for idx, (atom, symbol) in enumerate(self.output):
            mapped_atom = self.mapping[atom]
            if mapped_atom == TRUE:
                self.output_facts.append(symbol)
            elif mapped_atom > self.offset:
                mapped_atom -= self.offset
            else: # elif mapped_atom # init external
                symbol = clingo.Function(symbol.name[:-1], symbol.arguments)
            self.output[idx] = (mapped_atom, symbol)

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
        self.assigned_externals = {}
        self.output = output
        self.output_facts = output_facts
        self.init = init
        # rest
        self.ctl = None
        self.backend = None
        self.steps = 0

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
    generator_class = DLPGenerator
    if len(sys.argv)>=2 and sys.argv[1] == "simple":
        print("SIMPLIFIER")
        generator_class = DLPGeneratorSimplifier
    generator = generator_class(
        files = [],
        adds  = [("base", [], base)],
        parts = [("base", [])],
        options = ["0"],
    )
    dynamic_lp = generator.run()
    #print(generator)
    ctl = clingo.Control(["0"])
    dynamic_lp.start(ctl)
    steps = 4
    dynamic_lp.ground(3)
    dynamic_lp.ground(1)
    for i in range(1,steps):
        dynamic_lp.release_external(i, clingo.parse_term("last"))
    dynamic_lp.assign_external(steps, clingo.parse_term("last"), True)
    with ctl.solve(
        assumptions = dynamic_lp.get_assumptions(), yield_=True
    ) as handle:
        answers = 0
        for m in handle:
            answers += 1
            print("Answer: {}".format(answers))
            answer = dynamic_lp.get_answer(m,steps)
            print(" ".join(["{}:{}".format(x,y) for x,y in answer]))
        if not answers:
            print("UNSATISFIABLE")

if __name__ == "__main__":
    main()

