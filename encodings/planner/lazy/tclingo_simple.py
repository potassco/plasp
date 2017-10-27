#!/bin/usr/python

import clingo
import sys

STR_UNSAT = "error: input program is UNSAT"
INIT = "init"

#
# Syntax Restriction:
#   - primed atoms are not allowed in the heads
#
# Semantics:
#   - the transition is defined by the stable models of the program
#   - the possible initial states are defined by the set of primed externals
#   - normal (not primed) externals work normally
#
# Extensions:
#   - init/1 defines the initial situation
#   - external last/0 is set to true only at the last step
#

# example program
base = """
% background
fluent(loaded). fluent(alive).
value(true). value(false).
action(load). action(shoot).


% transition generation
holds(loaded,true) :- occ(load).
holds(alive,false) :- occ(shoot), holds'(loaded,true).
holds(F,V) :- holds'(F,V), not holds(F,VV) : value(VV), VV != V.
1 { occ(A) : action(A) } 1.
%:- holds(alive,true), last.

#show holds/2.
#show occ/1.

% state generation
#external holds'(F,V) : fluent(F), value(V).
#external last.

% init
init(holds(loaded,false)).
init(holds(alive,true)).
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
        self.max = 0
        self.rules = []
        self.weight_rules = []
        self.externals = []
        self.next = {}
        self.offset = 0
        self.mapping = []
        self.init_externals = []
        self.normal_externals = []
        self.output = set()
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
        self.simplify_program()
        self.set_externals()
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
            self.init_externals, self.normal_externals, self.output, self.init
        )

    def simplify_program(self):
        return

    def set_externals(self):
        self.externals = [x for x in self.ctl.symbolic_atoms if x.is_external]

    def get_next_literal(self, symbol, literal):
        try:
            next_symbol = clingo.Function(symbol.name[:-1], symbol.arguments)
            return self.ctl.symbolic_atoms[next_symbol].literal
        except Exception:
            return literal

    def set_next(self):
        self.next = {x.literal : self.get_next_literal(x.symbol, x.literal)
                     for x in self.externals if
                        len(x.symbol.name) and
                        x.symbol.name[-1] == "'"
                    }

    def set_mapping(self):
        self.mapping = [0] * (self.max + 1)
        self.offset = self.max # TODO: check if this works
        for i in range(1, self.max + 1):
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
                self.init_externals.append((mapping, i.symbol))
            else:                      # normal external
                self.normal_externals.append((mapping-self.offset, i.symbol))
                self.rules.append((True, [mapping], []))

    def set_output(self):
        with self.ctl.solve(yield_=True) as handle:
            for m in handle:
                return
        raise Exception(STR_UNSAT)

    def set_init(self):
        self.init = [self.mapping[
                         self.ctl.symbolic_atoms[
                             clingo.Function(i.symbol.arguments[0].name+"'",
                                             i.symbol.arguments[0].arguments
                                            )
                         ].literal
                     ]
                     for i in self.ctl.symbolic_atoms
                         if i.symbol.name == INIT
        ]

    #
    # observer
    #

    def update_max(self, _list):
        for i in _list:
            if i>self.max:
                self.max = i

    def rule(self, choice, head, body):
        self.update_max(head)
        self.update_max(body)
        self.rules.append((choice, head, body))

    def weight_rule(self, choice, head, lower_bound, body):
        self.update_max(head)
        self.update_max([x for x,y in body])
        self.weight_rules.append((choice, head, lower_bound, body))

    def external(self, atom, value):
        self.update_max([atom])

    def output_atom(self, symbol, atom):
        mapped_atom = self.mapping[atom]
        if mapped_atom > self.offset: # normal external
            mapped_atom -= self.offset
        elif mapped_atom:             # init external
            symbol = clingo.Function(symbol.name[:-1], symbol.arguments)
        self.output.add((mapped_atom, symbol))

    #
    # __str__
    #

    def __str__(self):
        out = ""
        out += "\nMAPPING\n" + "\n".join(
            ["{}:{}".format(i, item)
             for i, item in enumerate(self.mapping)]
        )
        out += "\nRULES\n" + "\n".join(
            ["{}:{}:{}".format(
                i[0], i[1], i[2]
            ) for i in self.rules]
        )
        out += "\nWEIGHT RULES\n" + "\n".join(
            ["{}:{}:{}:{}".format(
                i[0], i[1], i[2], i[3]
            ) for i in self.weight_rules]
        )
        out += "\nINIT EXTERNALS\n" + "\n".join(
            ["{}:{}".format(x, y) for x, y in sorted(self.init_externals)]
        )
        out += "\nNORMAL EXTERNALS\n" + "\n".join(
            ["{}:{}".format(x, y) for x, y in sorted(self.normal_externals)]
        )
        out += "\nOUTPUT\n" + "\n".join(
            ["{}:{}".format(x, y) for x, y in sorted(self.output)]
        )
        out += "\nINIT\n" + "\n".join(
            ["{}".format(x) for x in sorted(self.init)]
        )
        return out


class DynamicLogicProgram:

    def __init__(self, offset, rules, weight_rules,
                 init_externals, normal_externals, output, init):
        # init
        self.offset = offset
        self.rules = rules
        self.weight_rules = weight_rules
        self.init_externals = init_externals
        self.normal_externals = normal_externals
        self.assigned_externals = {}
        self.output = output
        self.init = init
        # rest
        self.ctl = None
        self.backend = None

    def start(self, ctl):
        self.ctl = ctl
        self.backend = ctl.backend
        #self.ctl.register_observer(self)
        # TODO: tell ROLAND to reorder
        for atom in self.init:
            self.backend.add_rule([atom], [], False)

    def ground(self, start, end):
        for step in range(start, end+1):
            offset = (step-1)*self.offset
            for rule in self.rules:
                self.backend.add_rule(
                    [x+offset for x in rule[1]],
                    [x+offset for x in rule[2] if x >0] +
                    [x-offset for x in rule[2] if x<=0],
                    rule[0]
                )
            for rule in self.weight_rules:
                self.backend.add_weight_rule(
                    [x+offset for x in rule[1]],
                    rule[2],
                    [(x+offset,y) for x, y in rule[3] if x >0] +
                    [(x-offset,y) for x, y in rule[3] if x<=0],
                    rule[0]
                )
            for atom, symbol in self.normal_externals:
                self.backend.add_rule([],[-(atom+offset+self.offset)],False)
                self.assigned_externals[(step, symbol)] = False #atom + (step*self.offset))

    def assign_external(self, step, symbol, value):
        del self.assigned_externals[(step, symbol)]
        if value is not None:
            self.assigned_externals[(step, symbol)] = value

    def release_external(self, step, symbol):
        del self.assigned_externals[(step, symbol)]
        #self.backend.add_rule([], [atom], False)

    def get_answer(self, model, step):
        out = []
        for i in range(step+1):
            for atom, symbol in self.output:
                if model.is_true(atom+(i*self.offset)):
                    out.append((i, symbol))
        return out

    # TODO
    def get_assumptions(self):
        return []

    def rule(self, choice, head, body):
        print("{}:{}:{}".format(choice, head, body))

    def weight_rule(self, choice, head, lower_bound, body):
        print("{}:{}:{}:{}".format(choice, head, lower_bound, body))

    def external(self, atom, value):
        print("{}:{}".format(atom, value))

def main():
    generator = DLPGenerator(
        files = [],
        adds  = [("base", [], base)],
        parts = [("base", [])],
        options = []
    )
    dynamic_lp = generator.run()
    ctl = clingo.Control(["0"])
    dynamic_lp.start(ctl)
    steps = 3
    dynamic_lp.ground(1,steps)
    with ctl.solve(
        assumptions = dynamic_lp.get_assumptions(), yield_=True
    ) as handle:
        answers = 0
        for m in handle:
            answers += 1
            print("Answer: {}".format(answers))
            answer = dynamic_lp.get_answer(m,steps)
            print(" ".join(["{}:{}".format(x,y) for x,y in answer]))

if __name__ == "__main__":
    main()

