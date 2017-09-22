#!/bin/usr/python

import clingo
import sys

LAST = "last"
STR_UNSAT = "error: input program is UNSAT"

base = """
% background
fluent(loaded). fluent(alive).
value(true). value(false).
action(load). action(shoot).

% state generation
#external holds'(F,V) : fluent(F), value(V).
#external last.

% transition generation
holds(loaded,true) :- occ(load).
holds(alive,false) :- occ(shoot), holds'(loaded,true).
holds(F,V) :- holds'(F,V), not holds(F,VV) : value(VV), VV != V.
{ occ(A) : action(A) } 1.
"""


def _on_model(m):
    pass #print m.symbols(shown=True)

class DynamicLPGenerator:

    def __init__(self, files = [], adds = [], parts = [], options = []):
        # input
        self.files = files
        self.adds = adds
        self.parts = parts
        self.options = options
        # rest
        self.ctl = None
        self.rules = []
        self.weight_rules = []
        self.max = 0
        self.external_atoms = {}
        self.external_symbols = {}
        self.last = None
        self.true = set() # proved true
        self.false = set()
        self.uk = set()
        self.cautious  = set()

    def run(self):
        # preliminaries
        ctl = self.ctl = clingo.Control(self.options)
        ctl.register_observer(self)
        for i in self.files:
            ctl.load(i)
        for i in self.adds:
            ctl.add(i[0], i[1], i[2])
        ctl.ground(self.parts)
        print(self.true)
        print(self)
        # analyze
        self.set_externals()
        self.set_consequences()
        self.uk = set(range(1,self.max+1)).difference(
            self.true, self.false, self.external_atoms.keys()
        )

        self.set_truth()
        print(self.max)
        print(self.external_atoms)
        print(self.external_symbols)
        #print(" ".join([str(idx) for idx, x in enumerate(brave) if x]))
        #print(" ".join([str(idx) for idx, x in enumerate(cautious) if x]))
        print(self.true)
        print(self.false)
        print(self.uk)
        return None

    def get_next(self, symbol):
        try:
            # TODO: check formats and special cases
            next_symbol = clingo.Function(symbol.name[:-1], symbol.arguments)
            return self.ctl.symbolic_atoms[next_symbol].literal
        except Exception:
            raise Exception("error: error with external {}".format(symbol))

    def set_externals(self):
        for i in self.ctl.symbolic_atoms:
            if i.is_external:
                if i.symbol.name == LAST:
                    self.last = (i.literal, i.symbol)
                else:
                    _next = self.get_next(i.symbol)
                    self.external_atoms[i.literal] = (i.symbol, _next)
                    self.external_symbols[i.symbol] = (i.literal, _next)

    def get_consequences(self, opt, true):
        self.ctl.configuration.solve.enum_mode = opt
        conseq = [False] * (self.max+1)
        conseq_size = 0
        with self.ctl.solve(yield_=True) as handle:
            last = None
            for m in handle:
                last = m
            if last is None:
                raise Exception(STR_UNSAT)
            if true:
                symbols = last.symbols(atoms=True)
            else:
                symbols = last.symbols(atoms=True, complement=True)
            return set([self.ctl.symbolic_atoms[x].literal for x in symbols])

    # requires set_externals() called before
    def set_consequences(self):
        for i in self.external_symbols.keys():
            self.ctl.assign_external(i, None)
        self.false = self.get_consequences("brave", False)
        self.cautious = self.get_consequences("cautious", True)

    def simplify_rules(self, rules, choice, head, body, true, false):
        new_rules = []
        for rule in rules:
            # heads true
            head_true = true.intersection(rule[head])
            if head_true and \
               (not rule[choice] or len(head_true)==len(rule[head])):
                continue # disjunctive head or full choice
            for h in head_true:
                rule[head].remove(h)
            # heads false
            head_false = false.intersection(rule[head])
            if head_false and rule[choice] and len(head_false)==len(rule[head]):
                continue # full choice
            for h in head_false:
                rule[head].remove(h)
            # normal bodies
            if body == 2:
                neg_body = [-x for x in rule[body]]
                if true.intersection(neg_body):
                    continue # true negated in body
                for b in true.intersection(rule[body]):
                    rule[body].remove(b)
                if false.intersection(rule[body]):
                    continue # false in body
                for b in false.intersection(neg_body):
                    rule[body].remove(-b)
            # weight bodies
            else:
                limit = rule[1]
                wbody = { abs(x) : (x,y) for x,y in rule[body]}
                true_atoms = true.intersection(wbody.keys())
                for atom in true_atoms:
                    rule[body].remove(wbody[atom])
                    if wbody[atom][0]>=0:
                        limit -= wbody[atom][1]
                false_atoms = false.intersection(wbody.keys())
                for atom in false_atoms:
                    rule[body].remove(wbody[atom])
                    if wbody[atom][0]<0:
                        limit -= wbody[atom][1]
                if limit != rule[1]:
                    new_rules.append((rule[0], rule[1], limit, rule[3]))
                    continue
            new_rules.append(rule)
        return new_rules

    def get_true_and_heads(self, rules, choice, head, body):
        new_rules, new_true, heads = [], set(), set()
        for rule in rules:
            if not rule[choice] and len(rule[head])==1 and not rule[body]:
                new_true.add(rule[head][0])
            else:
                new_rules.append(rule)
                heads.update(rule[head])
        return new_rules, new_true, heads

    def set_truth(self):
        true, false, uk = self.true, self.false, self.uk
        while True:
            self.rules = self.simplify_rules(
                self.rules, 0, 1, 2, true, false
            )
            self.weight_rules = self.simplify_rules(
                self.weight_rules, 0, 1, 3, true, false
            )
            self.rules, true1, heads1 = self.get_true_and_heads(
                self.rules, 0, 1, 2
            )
            self.weight_rules, true2, heads2 = self.get_true_and_heads(
                self.weight_rules, 0, 1, 3
            )
            true = true1.union(true2)
            false = uk.difference(true, heads1, heads2)
            uk = heads1.union(heads2)
            if not true and not false:
                break
            self.true.update(true)
            self.false.update(false)
        self.uk.difference_update(self.true, self.false)


    #
    # observer
    #

    # assuming all atoms appear in heads
    def update_max(self, _list):
        for i in _list:
            if i>self.max:
                self.max = i

    def rule(self, choice, head, body):
        print("{}:{}:{}".format(choice, head, body))
        self.update_max(head)
        if not choice and len(head)==1 and not body:
            self.true.add(head[0])
        else:
            self.rules.append((choice, head, body))

    def weight_rule(self, choice, head, lower_bound, body):
        print("{}:{}:{}:{}".format(choice, head, lower_bound, body))
        self.update_max(head)
        self.weight_rules.append((choice, head, lower_bound, body))

    #
    # __str__
    #

    def __str__(self):
        out = "\n".join(
            ["{}:{}:{}".format(
                i[0], i[1], i[2]
            ) for i in self.rules]
        )
        out += "\n" + "\n".join(
            ["{}:{}:{}:{}".format(
                i[0], i[1], i[2], i[3]
            ) for i in self.weight_rules]
        )
        #out += "\n" + str(self.atoms)
        #out += "\n" + str(self.externals)
        return out

class DynamicLogicProgram:

    def __init__(self,
                 externals, rules, weight_rules, output, output_facts, size):
        self.externals = externals
        self.rules = rules
        self.weight_rules = weight_rules
        self.output = output
        self.output_facts = output_facts
        self.size = size

def main():
    generator = DynamicLPGenerator(
        files = [],
        adds  = [("base", [], base)],
        parts = [("base", [])],
        options = []
    )
    dynamic_lp = generator.run()
    print(generator)

if __name__ == "__main__":
    main()
