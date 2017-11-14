# MIT License
# 
# Copyright (c) 2017 Javier Romero
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
# -*- coding: utf-8 -*-

#!/usr/bin/python

import os
import sys
import clingo
import json

#
# STATS
#

class Stats:


    def __init__(self):
        self.__width = 13



    def __ratio(self,x,y):
        return float(x)/float(y) if float(y)!=0 else 0


    def __percent(self,x,y):
        return 100*self.__ratio(x,y)


    def __print_key(self,key):
        return key + " "*(self.__width-len(key)) + ": "


    def __print_key_value(self,key,value):
        return self.__print_key(key) + value


    # requires Control initialized with --stats
    def summary(self,control,models=True):

        out = ""
        summary = control.statistics['summary']
        moreStr = "+" if int(summary['exhausted'])==0 else ""
        numEnum = int(summary['models']['enumerated'])
        if models:
            out += self.__print_key("Models")
            out += "{}{}\n".format(numEnum,moreStr)

        step = int(summary['call'])
        out += self.__print_key_value("Calls","{}\n".format(step+1))

        # return out if no stats
        if not 'accu' in control.statistics: return out
        times = control.statistics['accu']['times']
        out += self.__print_key("Time")
        totalTime = float(times['total'])
        solveTime = float(times['solve'])
        satTime   = float(times['sat'])
        unsatTime = float(times['unsat'])
        cpuTime   = float(times['cpu'])
        out += "{:.3f}s (Solving: {:.2f}s 1st Model: {:.2f}s Unsat: {:.2f}s)\n".format(totalTime,solveTime,satTime,unsatTime)
        out += self.__print_key_value("CPU Time","{:.3f}s".format(cpuTime))

        concurrency = int(summary['concurrency'])
        if concurrency > 1:
            out += "\n" + self.__print_key_value("Threads","{:<8}".format(concurrency))
            # when winner info becomes available: " (Winner: {})\n".format(winner)

        return out


    # requires Control initialized with --stats
    def statistics(self,control):

        # return "" if no stats
        if not 'accu' in control.statistics: return ""

        # choices...
        solver      = control.statistics['accu']['solving']['solvers']
        extra       = solver['extra']
        choices     = int(solver['choices'])
        domChoices  = int(extra['domain_choices'])
        conflicts   = int(solver['conflicts'])
        backjumps   = int(solver['conflicts_analyzed'])
        restarts    = int(solver['restarts'])
        avgRestart  = self.__ratio(backjumps,restarts)
        lastRestart = int(solver['restarts_last'])
        out = "\n" + self.__print_key_value("Choices","{:<8}".format(choices))
        if domChoices: out += " (Domain: {})".format(domChoices)
        out += "\n"
        out += self.__print_key_value("Conflicts","{:<8}".format(conflicts))
        out += " (Analyzed: {})\n".format(backjumps)
        out += self.__print_key_value("Restarts","{:<8}".format(restarts))
        if restarts>0: out += " (Average: {:.2f} Last: {})".format(avgRestart,lastRestart)
        out += "\n"

        # hccs
        hccTests   = int(extra['hcc_tests'])
        hccPartial = int(extra['hcc_partial'])
        if hccTests:
            out += self.__print_key_value("Stab. Tests","{:<8}".format(hccTests))
            out += " (Full: {} Partial: {})\n".format(hccTests-hccPartial,hccPartial)

        # model level
        models    = extra['models']
        modelLits = extra['models_level']
        avgModel  = self.__ratio(modelLits,models)
        if models:
            out += self.__print_key_value("Model-Level","{:<8.1f}\n".format(avgModel))

        # lemmas
        gps           = int(extra['guiding_paths'])
        gpLits        = int(extra['guiding_paths_lits'])
        avgGp         = self.__ratio(gpLits, gps)
        splits        = int(extra['splits'])
        sum           = int(extra['lemmas'])
        deleted       = int(extra['lemmas_deleted'])
        binary        = int(extra['lemmas_binary'])
        ternary       = int(extra['lemmas_ternary'])
        conflict      = int(extra['lemmas_conflict'])
        loop          = int(extra['lemmas_loop'])
        other         = int(extra['lemmas_other'])
        lits_conflict = int(extra['lits_conflict'])
        lits_loop     = int(extra['lits_loop'])
        lits_other    = int(extra['lits_other'])
        out += self.__print_key_value("Problems","{:<8}".format(gps))
        out += " (Average Length: {:.2f} Splits: {})\n".format(avgGp,splits)
        out += self.__print_key_value("Lemmas","{:<8}".format(sum))
        out += " (Deleted: {})\n".format(deleted)
        out += self.__print_key_value("  Binary","{:<8}".format(binary))
        out += " (Ratio: {:6.2f}%)\n".format(self.__percent(binary,sum))
        out += self.__print_key_value("  Ternary","{:<8}".format(ternary))
        out += " (Ratio: {:6.2f}%)\n".format(self.__percent(ternary,sum))
        out += self.__print_key_value("  Conflict","{:<8}".format(conflict))
        out += " (Average Length: {:6.1f} Ratio: {:6.2f}%) \n".format(self.__ratio(lits_conflict,conflict),self.__percent(conflict,sum))
        out += self.__print_key_value("  Loop","{:<8}".format(loop))
        out += " (Average Length: {:6.1f} Ratio: {:6.2f}%) \n".format(self.__ratio(lits_loop,loop),self.__percent(loop,sum))
        out += self.__print_key_value("  Other","{:<8}".format(other))
        out += " (Average Length: {:6.1f} Ratio: {:6.2f}%) \n".format(self.__ratio(lits_other,other),self.__percent(other,sum))

        # distributed...
        distributed = int(extra['distributed'])
        integrated  = int(extra['integrated'])
        if distributed or integrated:
            distRatio   = self.__ratio(distributed,conflict+loop)
            sumDistLbd  = int(extra['distributed_sum_lbd'])
            avgDistLbd  = self.__ratio(sumDistLbd,distributed)
            intRatio    =  self.__ratio(integrated,distributed)
            intImps     = int(extra['integrated_imps'])
            intJumps    = int(extra['integrated_jumps'])
            avgIntJump  = self.__ratio(intJumps,intImps)
            out += self.__print_key_value("  Distributed","{:<8}".format(distributed))
            out += " (Ratio: {:6.2f}% Average LBD: {:.2f}) \n".format(distRatio*100.0,avgDistLbd)
            out += self.__print_key_value("  Integrated","{:<8}".format(integrated))
            out += " (Ratio: {:6.2f}% ".format(intRatio*100.0) # for not accu: if not _accu: "("
            out += "Unit: {} Average Jumps: {:.2f})\n".format(intImps,avgIntJump)

        # jumps
        jumps       = extra['jumps']
        _jumps      = int(jumps['jumps'])
        bounded     = int(jumps['jumps_bounded'])
        jumpSum     = int(jumps['levels'])
        boundSum    = int(jumps['levels_bounded'])
        maxJump     = int(jumps['max'])
        maxJumpEx   = int(jumps['max_executed'])
        maxBound    = int(jumps['max_bounded'])
        jumped      = jumpSum - boundSum
        jumpedRatio = self.__ratio(jumped,jumpSum)
        avgBound    = self.__ratio(boundSum,bounded)
        avgJump     = self.__ratio(jumpSum,_jumps)
        avgJumpEx   = self.__ratio(jumped,_jumps)
        out += self.__print_key_value("Backjumps","{:<8}".format(_jumps))
        out += " (Average: {:5.2f} Max: {:>3} Sum: {:>6})\n".format(avgJump,maxJump,jumpSum)
        out += self.__print_key_value("  Executed","{:<8}".format(_jumps-bounded))
        out += " (Average: {:5.2f} Max: {:>3} Sum: {:>6} Ratio: {:6.2f}%)\n".format(avgJumpEx,maxJumpEx,jumped,jumpedRatio*100.0)
        out += self.__print_key_value("  Bounded","{:<8}".format(bounded))
        out += " (Average: {:5.2f} Max: {:>3} Sum: {:>6} Ratio: {:6.2f}%)\n".format(avgBound,maxBound,boundSum,100.0 - (jumpedRatio*100.0))
        out += "\n"

        # logic program
        lp                 = control.statistics['problem']['lp']

        # rules
        rOriginal          = int(lp['rules'])
        rules_normal       = int(lp['rules_normal'])
        rules_choice       = int(lp['rules_choice'])
        rules_minimize     = int(lp['rules_minimize'])
        rules_acyc         = int(lp['rules_acyc'])
        rules_heuristic    = int(lp['rules_heuristic'])
        rFinal             = int(lp['rules_tr'])
        rules_tr_normal    = int(lp['rules_tr_normal'])
        rules_tr_choice    = int(lp['rules_tr_choice'])
        rules_tr_minimize  = int(lp['rules_tr_minimize'])
        rules_tr_acyc      = int(lp['rules_tr_acyc'])
        rules_tr_heuristic = int(lp['rules_tr_heuristic'])
        out += self.__print_key_value("Rules","{:<8}".format(rFinal))
        if (rFinal != rOriginal):
            out += " (Original: {})".format(rOriginal)
        out += "\n"
        for i in [#["  Normal",   rules_normal,   rules_tr_normal],
                  ["  Choice",   rules_choice,   rules_tr_choice],
                  ["  Minimize", rules_minimize, rules_tr_minimize],
                  ["  Acyc", rules_acyc, rules_tr_acyc],
                  ["  Heuristic", rules_heuristic, rules_tr_heuristic]]:
            if i[2]:
                out += self.__print_key_value(i[0],"{:<8}".format(i[2]))
                if (i[2] != i[1]):
                    out += " (Original: {})".format(i[1])
                out += "\n"

        # atoms
        atoms              = int(lp['atoms'])
        auxAtoms           = int(lp['atoms_aux'])
        out += self.__print_key_value("Atoms","{:<8}".format(atoms))
        if (auxAtoms):
            out += " (Original: {} Auxiliary: {})".format(atoms-auxAtoms,auxAtoms)
        out += "\n"

        # disjunctions
        disjunctions       = int(lp['disjunctions'])
        disjunctions_tr    = int(lp['disjunctions_non_hcf'])
        if disjunctions:
            out += self.__print_key_value("Disjunctions","{:<8}".format(disjunctions_tr))
            out += " (Original: {})\n".format(disjunctions)

        # bodies
        bFinal             = int(lp['bodies_tr'])
        bOriginal          = int(lp['bodies'])
        count_bodies       = int(lp['count_bodies'])
        count_bodies_tr    = int(lp['count_bodies_tr'])
        sum_bodies         = int(lp['sum_bodies'])
        sum_bodies_tr      = int(lp['sum_bodies_tr'])
        out += self.__print_key_value("Bodies","{:<8}".format(bFinal))
        if (bFinal != bOriginal):
            out += " (Original: {})".format(bOriginal)
        out += "\n"
        for i in [["  Count", count_bodies, count_bodies_tr],
                  ["  Sum",   sum_bodies,   sum_bodies_tr  ]]:
            if i[1]:
                out += self.__print_key_value(i[0],"{:<8}".format(i[2]))
                if (i[2] != i[1]):
                    out += " (Original: {})".format(i[1])
                out += "\n"

        # equivalences
        eqs                = int(lp['eqs'])
        eqsAtom            = int(lp['eqs_atom'])
        eqsBody            = int(lp['eqs_body'])
        eqsOther           = int(lp['eqs_other'])
        if eqs > 0:
            out += self.__print_key_value("Equivalences","{:<8}".format(eqs))
            out += " (Atom=Atom: {} Body=Body: {} Other: {})\n".format(eqsAtom,eqsBody,eqsOther)

        # sccs
        sccs       = int(lp['sccs'])
        nonHcfs    = int(lp['sccs_non_hcf'])
        ufsNodes  = int(lp['ufs_nodes'])
        gammas     = int(lp['gammas'])
        out += self.__print_key("Tight")
        if sccs==0: out += "Yes"
        # for supported models: elif sccs == PrgNode:noScc
        else: out += "{:<8} (SCCs: {} Non-Hcfs: {} Nodes: {} Gammas: {})".format("No",sccs,nonHcfs,ufsNodes,gammas)
        out += "\n"

        # problem
        gen        = control.statistics['problem']['generator']
        vars       = int(gen['vars'])
        eliminated = int(gen['vars_eliminated'])
        frozen     = int(gen['vars_frozen'])
        binary     = int(gen['constraints_binary'])
        ternary    = int(gen['constraints_ternary'])
        sum        = int(gen['constraints']) + binary + ternary
        acycEdges  = int(gen['acyc_edges'])
        out += self.__print_key_value("Variables","{:<8}".format(vars))
        out += " (Eliminated: {:>4} Frozen: {:>4})\n".format(eliminated,frozen)
        out += self.__print_key_value("Constraints","{:<8}".format(sum))
        out += " (Binary: {:5.1f}% Ternary: {:5.1f}% Other: {:5.1f}%)\n".format(self.__percent(binary,sum),self.__percent(ternary,sum),self.__percent(sum-binary-ternary,sum))

        return out


program = """

% pigeonhole problem
#const n=8.
pigeon(1..n+1). box(1..n).
1 { in(X,Y) : box(Y) } 1 :- pigeon(X).
:- 2 { in(X,Y) : pigeon(X) },  box(Y).

% heuristic
#heuristic in(X,Y) : pigeon(X), box(Y). [1,true]

% disjunction
a | b.  a :- b.  b :- a.

% SAT
box(n+1).

"""

satisfiable = False
def on_model(model):
    global satisfiable
    sys.stdout.write("Answer: 1\n{}\n".format(str(model)))
    satisfiable = True


def run():

    # set options
    options = "-t4 --stats --heuristic=Domain"
    #options = ""

    # with Control()
    control = clingo.Control(options.split())
    control.add("a",[],program)
    control.ground([("a",[])])
    control.solve(on_model=on_model)
    if satisfiable: sys.stdout.write("SATISFIABLE\n")
    else:           sys.stdout.write("UNSATISFIABLE\n")
    sys.stdout.write(Stats().summary(control)+"\n")
    sys.stdout.write(Stats().statistics(control)+"\n")

    # with $clingo
    file = "tmp.lp"
    with open(file, "w") as text_file:
        text_file.write(program)
    os.system("clingo {} {}; rm {}".format(options,file,file))

if __name__ == "__main__":
    run()

