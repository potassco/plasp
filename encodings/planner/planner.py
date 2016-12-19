#!/usr/bin/python

#
# IMPORTS
#

import clingo
import sys
import argparse
import re


#
# Global variables and functions
#

verbose_option = False

def do_print(string,verbose=True):
    if verbose or verbose_option:
        print string



#
# SCHEDULERS
#


class Scheduler:


    def __init__(self):
        pass


    def next(self,result):
        return 0,0


class Sequential_Scheduler(Scheduler):


    def __init__(self,start,inc,time):
        self.__length = start
        self.__inc    = inc
        self.__time   = time
        self.__runs   = []


    def set_size(self,size):
        self.__size = size


    def set_inc(self,inc):
        self.__inc  = inc


    def next(self,result):
        if result == None:
            self.__runs = [(self.__length,self.__time)]
            return self.__runs[0][0], self.__runs[0][1]
        if result.interrupted or result.satisfiable:
            return self.__runs[0][0], self.__runs[0][1]
        self.__length = self.__length + self.__inc
        self.__runs = [(self.__length,self.__time)]
        return self.__runs[0][0], self.__runs[0][1]



class A_Scheduler(Scheduler):


    def __init__(self,start,inc,time,size):
        self.__length = start
        self.__inc    = inc
        self.__time   = time
        self.__size   = size
        self.__runs   = []


    def set_size(self,size):
        self.__size = size


    def set_inc(self,inc):
        self.__inc  = inc

    def next(self,result):
        if result == None:     # add all runs
            self.__runs   = [ (self.__length + (i*self.__inc), self.__time) for i in range(self.__size)]
            self.__length = self.__length + ((self.__size-1)*self.__inc)
        elif result.interrupted or result.satisfiable: # move to the end of the queue
            self.__runs.append(self.__runs[0])
            self.__runs = self.__runs[1:]
        else:
            self.__length = self.__length + self.__inc
            self.__runs.append((self.__length,self.__time))
            self.__runs = self.__runs[1:]
        do_print("Queue:\t\t " + str(self.__runs),False)
        return self.__runs[0][0], self.__runs[0][1]



#
# SOLVER
#

OCCURS       = "occurs"
OCCURS_ARITY = 2
STEP         = "step"
CHECK        = "check"
QUERY        = "query"
NO_ACTION    = "no_action"

class Solver:


    def __init__(self,ctl):
        self.__ctl     = ctl
        self.__length  = 0


    def __on_model(self,m):
        self.__plan = [ i for i in m.symbols(atoms=True) if i.name==OCCURS and len(i.arguments)==OCCURS_ARITY ]
        do_print("Answer:\n" + str(m))


    def __on_finish(self,result):
        self.__result = result


    def solve(self,length,time):

        do_print("Grounded Until:\t {}".format(self.__length),False)

        # ground if necessary
        if self.__length < length:
            parts = [(STEP,[t]) for t in range(self.__length+1,length+1)]
            parts = parts + [(CHECK,[length])]
            self.__ctl.release_external(clingo.Function(QUERY,[self.__length]))
            do_print("Grounding...")
            do_print(str(parts),False)
            self.__ctl.ground(parts)
            self.__ctl.assign_external(clingo.Function(QUERY,[length]),True)
            self.__length = length

        # no actions if necessary
        elif length < self.__length:
            do_print("Blocking actions...",False)
            for t in range(length+1,self.__length+1):
                self.__ctl.assign_external(clingo.Function(NO_ACTION,[t]),True)

        # solve with time limit
        do_print("Solving...")
        self.__plan = []
        future = self.__ctl.solve_async(on_model=self.__on_model,on_finish=self.__on_finish) # sets self.__result and self.__plan
        future.wait(time) or future.cancel()
        do_print(str(self.__result)+"\n",False)

        # undo no actions if necessary
        if length < self.__length:
            do_print("Unblocking actions...",False)
            for t in range(length+1,self.__length+1):
                self.__ctl.assign_external(clingo.Function(NO_ACTION,[t]),False)

        # return
        return self.__result, self.__plan



#
# PLANNER
#

BASE = "base"
PROGRAMS = """
#program step(t).

% no action
#external no_action(t).
:- occurs(A,t), no_action(t).


#program check(t).
#external query(t).

#program heuristics(t).
#const max_plan=10000.
#const heuristics=off.
#heuristic holds(F,V,t) : holds(F,V,t-1), heuristics=on. [max_plan-t,true]
"""

class Planner:

    def run(self,options,clingo_options):

        #ctl = clingo.Control(["--warn=no-atom-undefined"])
        ctl = clingo.Control(clingo_options)

        # input files
        for i in options['files']:
            ctl.load(i)
        if options['read_stdin']:
            ctl.add(BASE,[],sys.stdin.read())

        # step, check and heuristic programs
        ctl.add(BASE,[],PROGRAMS)

        # initialize
        ctl.ground([(BASE,[])])
        scheduler = A_Scheduler(5,5,1,5) # start,inc,time
        if options['S'] is not None:
            s = [int(x) for x in options['S']]
            scheduler = Sequential_Scheduler(s[0],s[1],s[2])      # start,inc,time
        elif options['A'] is not None:
            a = [int(x) for x in options['A']]
            scheduler = A_Scheduler(a[0],a[1],a[2],a[3]) # start,inc,time,queue
        solver    = Solver(ctl)

        # loop
        i=1
        result = None
        while True:
            do_print("Iteration "+str(i),False)
            i += 1
            length, time = scheduler.next(result)
            result, plan = solver.solve(length,time)
            if result.satisfiable:
                do_print("SATISFIABLE")
                return



#
# ARGUMENT PARSER
#

VERSION = "0.0.1"

class PlannerArgumentParser:

    clingo_help = """
Clingo Options:
  --<option>[=<value>]\t: Set clingo <option> [to <value>]

    """

    usage = "planner.py [options] [files]"

    epilog = """
Default command-line:
planner.py -A 5 5 1 5

planner is part of plasp in Potassco: https://potassco.org/
Get help/report bugs via : https://potassco.org/support
    """

    def run(self):

        # version
        _version = "planner.py version " + VERSION
        print _version

        # command parser
        _epilog = self.clingo_help + "\nusage: " + self.usage + self.epilog
        cmd_parser = argparse.ArgumentParser(
            usage=self.usage,epilog=_epilog,formatter_class=argparse.RawDescriptionHelpFormatter,
            add_help=False)
        # basic
        basic = cmd_parser.add_argument_group('Basic Options')
        basic.add_argument('-h','--help',action='help',help='Print help and exit')
        basic.add_argument('-',dest='read_stdin',action='store_true',help=argparse.SUPPRESS)
        basic.add_argument('-c','--const',dest='constants',action="append",help=argparse.SUPPRESS,default=[])
        basic.add_argument('-v','--verbose',dest='verbose',action="store_true",help="Be a bit more verbose")
        # scheduler
        scheduler = cmd_parser.add_argument_group('Scheduler Options')
        scheduler.add_argument('-S',nargs=3,dest='S',help="Sequential Scheduler starting at time point <s>,\
                               incrementing <i> time points, and solving for <t> seconds",metavar=('<s>','<i>','<t>'),default=None)
        scheduler.add_argument('-A',nargs=4,dest='A',help="A Scheduler starting at time point <s>, incrementing <i> time points,\
                                solving for <t> seconds with a queue of <q> elements (Default: -A 5 5 1 5)",
                                metavar=('<s>','<i>','<t>','<q>'),default=None)

        # parse
        options, unknown = cmd_parser.parse_known_args()
        options = vars(options)

        # separate files, and clingo options
        options['files'], clingo_options = [], []
        for i in unknown:
            if (re.match(r'^-',i)): clingo_options.append(i)
            else:                   options['files'].append(i)
        if options['files'] == []: options['read_stdin'] = True

        # add constants to clingo_options
        for i in options['constants']:
            clingo_options.append("-c {}".format(i))

        # set verbose
        global verbose_option
        verbose_option = options['verbose']

        # return
        return options, clingo_options



#
# MAIN
#

if __name__ == "__main__":
    options, clingo_options = PlannerArgumentParser().run()
    Planner().run(options,clingo_options)

