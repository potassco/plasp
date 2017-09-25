#!/bin/usr/python

import clingo
import sys

STR_UNSAT = "error: input program is UNSAT"

#
# primed atoms are not allowed in the heads
#

base = """
% background
fluent(loaded). fluent(alive).
value(true). value(false).
action(load). action(shoot).


% transition generation
holds(loaded,true) :- occ(load).
holds(alive,false) :- occ(shoot), holds'(loaded,true).
holds(F,V) :- holds'(F,V), not holds(F,VV) : value(VV), VV != V.
{ occ(A) : action(A) } 1.
:- holds(alive,true), last.

#show holds/2.
#show occ/1.

% state generation
#external holds'(F,V) : fluent(F), value(V).
#external last.
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
        self.set_next()
        self.set_mapping()
        self.map_rules()
        self.map_weight_rules()
        self.handle_externals()
        self.set_output()
        # return
        return DynamicLogicProgram(
            self.offset, self.rules, self.weight_rules,
            self.init_externals, self.normal_externals, self.output
        )

    def set_externals(self):
        self.externals = [x for x in self.ctl.symbolic_atoms if x.is_external]

    def get_next_literal(self, symbol, literal):
        try:
            next_symbol = clingo.Function(symbol.name[:-1], symbol.arguments)
            return self.ctl.symbolic_atoms[next_symbol].literal
        except Exception:
            return literal

    # TODO: check special cases
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
            for i, head in enumerate(rule[1]):
                rule[1][i] = self.mapping[head]
            for i, head in enumerate(rule[2]):
                rule[2][i] = self.mapping[head]

    def map_weight_rules(self):
        for rule in self.weight_rules:
            for i, head in enumerate(rule[1]):
                rule[1][i] = self.mapping[head]
            for i, head in enumerate(rule[3]):
                rule[3][i] = (self.mapping[head[0]], head[1])

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
        return out

class DynamicLogicProgram:

    def __init__(self, offset, rules, weight_rules,
                 init_externals, normal_externals, output):
        self.offset = offset
        self.rules = rules
        self.weight_rules = weight_rules
        self.init_externals = init_externals
        self.normal_externals = normal_externals
        self.output = output

def main():
    generator = DLPGenerator(
        files = [],
        adds  = [("base", [], base)],
        parts = [("base", [])],
        options = []
    )
    dynamic_lp = generator.run()
    print(generator)

if __name__ == "__main__":
    main()
