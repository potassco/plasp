#!/bin/usr/python

import clingo
import sys

PAIR = "_pair"

base = """
% background
fluent(loaded). fluent(alive).
value(true). value(false).
action(shoot). action(load).

% state generation
#external holds(F,V,0) : fluent(F), value(V).
#external     occ(A,0) : action(A).

% pairing
_pair(holds(F,V,0),holds(F,V,1)) :- fluent(F), value(V).
       _pair(occ(A,0), occ(A,1)) :- action(A).

% transition generation
holds(loaded,true,1) :- occ(load,1).
holds(alive,false,1)  :- occ(shoot,1), holds(loaded,true,0).
holds(F,V,1) :- holds(F,V,0), not holds(F,VV,1) : value(VV), VV != V.
{ occ(A,1) : action(A) } 1.
"""

def _on_model(m):
    pass #print m.symbols(shown=True)

class GroundProgramObserver:

    def __init__(self):
        self.rules    = []
        self.tr_rules = []
        self.atoms = {}
        self.externals = set()
        self.weight_rules = []
        self.tr_weight_rules = []
        self.listen = True
        self.pairs = {}
        self.symbols = {}

    def rule(self, choice, head, body):
        if self.listen:
            self.rules.append((choice, head, body))

    def weight_rule(self, choice, head, lower_bound, body):
        if self.listen:
            self.weight_rules.append((choice, head, lower_bound, body))

    def set_listen(self, _bool):
        self.listen = _bool

    def external(self, atom, value):
        self.externals.add(atom)

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
            ) for i in self.tr_rules]
        )
        out += "\n" + "\n".join(
            ["{}:{}:{}:{}".format(
                i[0], self.tr(i[1]), i[2], i[3]
            ) for i in self.tr_weight_rules]
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


    def add_tr_rule(self, r):
        self.tr_rules.append(r)

    def add_tr_weight_rule(self, r):
        self.tr_weight_rules.append(r)

    def select_transition_rules(self):
        facts = set()
        while True:
            rules = []
            for rule in self.rules:
                if not rule[0] and not rule[2]:
                    for atom in rule[1]:
                        facts.add(atom)
                else:
                    rules.append(rule)
            self.rules = rules
            if not facts:
                break
            # assuming facts may only appear in bodies
            for rule in self.rules:
                for atom in rule[2]:
                    if atom in facts:
                        rule[2].remove(atom)
            for rule in self.weight_rules:
                for atom, weight in rule[3]:
                    if atom in facts:
                        rule[3].remove(atom)
            facts = set()
        self.tr_rules = self.rules
        self.tr_weight_rules = self.weight_rules
        return
        # not used
        externals = self.externals
        facts = set()
        for r in self.rules:
            done = False
            for h in r[1]:
                if h not in externals:
                    self.add_tr_rule(r)
                    done = True
                    break
            if done:
                continue
            for b in r[2]:
                if b not in externals:
                    self.add_tr_rule(r)
                    break
        for r in self.weight_rules:
            done = False
            for h in r[1]:
                if h not in externals:
                    self.add_tr_weight_rule(r)
                    done = True
                    break
            if done:
                continue
            for b in r[3]:
                if b[0] not in externals:
                    self.add_tr_weight_rule(r)
                    break
        del(self.rules)
        del(self.weight_rules)
        #TODO: delete facts from program

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
        iter_offset = 0
        externals = self.get_externals()
        for it in range(iter):
            old_offset   = iter_offset
            iter_offset += offset
            for r in self.tr_rules:
                head, body, choice = self.update_rule(
                    r, old_offset, iter_offset, externals
                )
                print("{}:{}:{}".format(choice, head, body))
                backend.add_rule(head, body, choice)
            for r in self.tr_weight_rules:
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
    observer.select_transition_rules()
    print(observer)
    observer.update(2,100,ctl.backend)

if __name__ == '__main__': 
    main()

