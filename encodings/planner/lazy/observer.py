#!/bin/usr/python

import clingo
import sys

PAIR = "_pair"

base = """
% background
fluent(loaded). fluent(alive).
value(true). value(false).
action(load). action(shoot).

% state generation
#external holds'(F,V) : fluent(F), value(V).

% transition generation
holds(loaded,true) :- occ(load).
holds(alive,false) :- occ(shoot), holds'(loaded,true).
holds(F,V) :- holds'(F,V), not holds(F,VV) : value(VV), VV != V.
{ occ(A) : action(A) } 1.
"""

def _on_model(m):
    pass #print m.symbols(shown=True)

class GroundProgramObserver:

    def __init__(self):
        self.rules = []
        self.atoms = {}
        self.externals = set()
        self.weight_rules = []
        self.listen = True
        self.pairs = {}
        self.symbols = {}
        self.facts = set()
        self.max = 0

    def rule(self, choice, head, body):
        if self.listen:
            # assuming all numbers appear in heads
            for i in head:
                if i > self.max:
                    self.max = i
            self.rules.append((choice, head, body))

    def weight_rule(self, choice, head, lower_bound, body):
        if self.listen:
            # assuming all numbers appear in heads
            for i in head:
                if i > self.max:
                    self.max = i
            self.weight_rules.append((choice, head, lower_bound, body))

    def set_listen(self, _bool):
        self.listen = _bool

    def external(self, atom, value):
        self.externals.add(atom)
        if atom > self.max:
            self.max = atom

    def atom_tr(self, atom):
        out = "not " if atom < 0 else ""
        return out + str(self.atoms.get(abs(atom), [abs(atom)])[0])

    def tr(self, _list):
        out = []
        for i in _list:
            out.append(self.atom_tr(i))
        return "[" + ",".join(out) + "]"

    def __str__(self):
        out = "\n".join(
            ["{}:{}:{}".format(
                i[0], self.tr(i[1]), self.tr(i[2])
            ) for i in self.rules]
        )
        out += "\n" + "\n".join(
            ["{}:{}:{}:{}".format(
                i[0], self.tr(i[1]), i[2], i[3]
            ) for i in self.weight_rules]
        )
        out += "\n" + str(self.atoms)
        out += "\n" + str(self.externals)
        return out

    def output_atom(self, symbol, atom):
        self.atoms.setdefault(atom, []).append(str(symbol))
        self.symbols[str(symbol)] = atom
        try:
            if symbol.name == PAIR:
                self.pairs[str(symbol.arguments[0])] = str(symbol.arguments[1])
        except Exception:
            pass

    def get_facts(self):
        rules = []
        for rule in self.rules:
            if not rule[0] and not rule[2]:
                for atom in rule[1]:
                    self.facts.add(atom)
            else:
                rules.append(rule)
        self.rules = rules
        if not self.facts:
            return
        # remove elements from rules
        # assuming facts may only appear in bodies
        for rule in self.rules:
            for atom in rule[2]:
                if atom in self.facts:
                    rule[2].remove(atom)
        for rule in self.weight_rules:
            for atom, weight in rule[3]:
                if atom in self.facts:
                    rule[3].remove(atom)

    def update_rule(self, rule, offset0, offset1, externals):
        head, body, choice = [], [], rule[0]
        for i in rule[1]:
            if i in externals:
                head.append(externals[i]+offset0)
            else:
                head.append(i+offset1)
        for i in rule[2]:
            if i in externals:
                body.append(externals[i]+offset0)
            else:
                body.append(i+offset1)
        return head, body, choice

    def update_weight_rule(self, rule, offset0, offset1, externals):
        head, lower, body, choice = [], rule[2], [], rule[0]
        for i in rule[1]:
            if i in externals:
                head.append(externals[i]+offset0)
            else:
                head.append(i+offset1)
        for i in rule[3]:
            if i[0] in externals:
                body.append((externals[i[0]]+offset0, i[1]))
            else:
                body.append((i[0]+offset1, i[1]))
        return head, lower, body, choice

    def get_externals(self):
        out = {}
        for atom in self.externals:
            out[atom] = self.symbols[self.pairs[self.atoms[atom][0]]]
        return out

    def update(self, iter, offset, backend):
        atoms = [0] * (self.max+1)
        number = 1
        for i range(1,self.max+1):
            if i not in self.facts and i not in self.externals:
                atoms[i] = number
                number += 1
        out = {}
        for i in self.externals:
            out[atom] = self.symbols[self.pairs[self.atoms[atom][0]]]
            atoms[i] = atoms[out[atom]] - (number-1)
        # for i in self.rules: ... 
        # for i in self.weight_rules: ... 
        
        iter_offset = 0
        externals = self.get_externals()
        for it in range(iter):
            old_offset   = iter_offset
            iter_offset += offset
            for r in self.rules:
                head, body, choice = self.update_rule(
                    r, old_offset, iter_offset, externals
                )
                print("{}:{}:{}".format(choice, head, body))
                backend.add_rule(head, body, choice)
            for r in self.weight_rules:
                head, lower, body, choice = self.update_weight_rule(
                    r, old_offset, iter_offset, externals
                )
                print("{}:{}:{}:{}".format(choice, head, lower, body))
                backend.add_weight_rule(head, lower, body, choice)

def main():
    ctl = clingo.Control(["0"])
    observer = GroundProgramObserver()
    ctl.register_observer(observer)
    ctl.add("base",[],base)
    ctl.ground([("base",[])])
    ctl.solve(on_model=_on_model)
    observer.set_listen(False)
    observer.get_facts()
    print(observer)
    print(observer.max)
    print(len(observer.facts))
    print(len(observer.externals))
    #observer.update(2,100,ctl.backend)

if __name__ == '__main__': 
    main()

