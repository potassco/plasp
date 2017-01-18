#!/usr/bin/python


#
# IMPORTS
#

import clingo
import sys
import argparse
import re
from time import clock


#
# Global variables and functions
#


verbose_option = False
outf           = 0
def do_print(string,verbose=True):
    if verbose or verbose_option:
        print ("% " if outf==1 else "") + string


#
# memory_usage()
#
import os
def memory_usage():
    # data
    proc_status = '/proc/%d/status' % os.getpid()
    #proc_stat  = '/proc/%d/stat' % os.getpid()
    scale = {'kB': 1024.0, 'mB': 1,
             'KB': 1024.0, 'MB': 1}
    key = 'VmSize:'
    # get pseudo file  /proc/<pid>/status
    try:
        t = open(proc_status)
        v = t.read()
        t.close()
    except:
        return 0.0  # non-Linux?
    # get VmKey line e.g. 'VmSize:  9999  kB\n ...'
    i = v.index(key)
    v = v[i:].split(None, 3)  # whitespace
    if len(v) < 3:
        return 0.0  # invalid format?
    # return
    return int(float(v[1]) / scale[v[2]])




#
# SCHEDULERS
#


class Scheduler:


    def __init__(self):
        pass


    def next(self,result):
        return 0,0



class A_Scheduler(Scheduler):


    def __init__(self,start,inc,limit,size,propagate_unsat):
        self.__length          = start
        self.__inc             = inc
        self.__limit           = limit
        self.__size            = size
        self.__propagate_unsat = propagate_unsat
        self.__runs            = []


    def next(self,result):

        # START: add all runs
        if result == None:
            self.__runs   = [ self.__length+(i*self.__inc) for i in range(self.__size) ]
            self.__runs   = [ i for i in self.__runs if i<=self.__limit ]
            self.__length = self.__length + ((self.__size-1)*self.__inc)

        # UNKNOWN: enqueue and pop
        elif result.unknown:
            self.__runs.append(self.__runs[0])
            self.__runs = self.__runs[1:]

        # UNSAT
        else:
            if self.__propagate_unsat:                 # propagate unsat
                current_length = self.__runs[0]
                self.__runs = [ i for i in self.__runs if i>=current_length ]
            next_length = self.__length + self.__inc
            if next_length <= self.__limit:            # if inside the limit: enqueue next
                self.__length = next_length
                self.__runs.append(self.__length)
            self.__runs = self.__runs[1:]              # pop

        # print and return
        do_print("Queue:\t\t " + str(self.__runs),False)
        return self.__runs[0] if len(self.__runs)>0 else None



class Run:


    def __init__(self,index,length,effort,solve):
        self.index  = index
        self.length = length
        self.effort = effort
        self.solve  = solve


    def __repr__(self):
        return "("+",".join([str(i) for i in [self.index,self.length,self.effort,self.solve]])+")"



class B_Scheduler:


    def __init__(self,start,inc,limit,size,propagate_unsat,gamma):
        self.__index           = 0
        self.__start           = start
        self.__inc             = inc
        self.__limit           = limit
        self.__size            = size
        self.__propagate_unsat = propagate_unsat
        self.__gamma           = gamma
        self.__runs            = []
        self.__next_runs       = []


    def next(self,result):

        # if not first time
        if result is not None:
            # UNKNOWN: effort++, and append to __next_runs
            if result.unknown:
                self.__runs[0].effort += 1
                self.__next_runs.append(self.__runs[0])
            # UNSAT and propagate: reset __next_runs
            if result.unsatisfiable and self.__propagate_unsat:
                self.__next_runs = []
            # UNKNOWN or UNSAT: pop __runs
            self.__runs = self.__runs[1:]
            # move to __next_runs while not solve
            while self.__runs != [] and not self.__runs[0].solve:
                self.__next_runs.append(self.__runs[0])
                self.__runs = self.__runs[1:]

        # if no more runs
        if self.__runs == []:

            # if __next_runs is not empty: add to __runs
            if self.__next_runs != []:
                first = self.__next_runs[0]
                first.solve = True
                self.__runs = [ first ]
                for i in self.__next_runs[1:]:
                    i.solve = True if (i.effort < (((first.effort+1) * (self.__gamma ** (i.index - first.index)))+0.5)) else False
                    self.__runs.append(i)

            # else: add new to __runs
            else:
                self.__runs = [ Run(self.__index,self.__start+(self.__inc*self.__index),0,True) ]
                self.__index += 1
                first = self.__runs[0]
                if first.length > self.__limit: return None

            # reset __next_runs
            self.__next_runs = []

            # add next runs
            while (0.5 < ((first.effort+1) * (self.__gamma ** (self.__index - first.index)))):
                if len(self.__runs)>= self.__size: break
                next_length = self.__start+(self.__inc*self.__index)
                if next_length > self.__limit: break
                self.__runs.append(Run(self.__index,next_length,0,True))
                self.__index += 1

        # print and return
        do_print("Queue:\t\t " + str(self.__runs),False)
        return self.__runs[0].length



class C_Scheduler(Scheduler):


    def __init__(self,start,inc,limit,propagate_unsat):
        self.__length          = start
        self.__inc             = inc
        self.__limit           = limit
        self.__propagate_unsat = propagate_unsat
        self.__runs            = []


    def next(self,result):

        # START: add first run
        if result == None:
            self.__runs   = [ self.__length ]
            self.__length = int(self.__length*self.__inc)

        # ELSE: add new and handle last
        else:
            next_length = self.__length * self.__inc
            if next_length <= self.__limit:
                self.__runs.append(next_length)
            # UNKNOWN: append
            if result.unknown:
                self.__runs.append(self.__runs[0])
            # UNSAT: propagate_unsat
            elif self.__propagate_unsat:
                current_length = self.__runs[0]
                self.__runs = [ i for i in self.__runs if i>=current_length ]
            # pop
            self.__runs = self.__runs[1:]

        # print and return
        do_print("Queue:\t\t " + str(self.__runs),False)
        return self.__runs[0] if len(self.__runs)>0 else None



#
# SOLVER
#

OCCURS        = "occurs"
OCCURS_ARITY  = 2
STEP          = "step"
CHECK         = "check"
QUERY         = "query"
BASE          = "base"
NO_ACTION     = "no_action" # added
PROGRAMS      = """
#program step(t).

% no action
#external no_action(t).
:-     occurs(A,t),     no_action(t).

% some action
:- not occurs(_,t), not no_action(t).


#program check(t).
#external query(t).

"""
PLANNER_ON = "-c planner_on=true" # added


class Solver:


    def __init__(self,ctl,options):

        self.__ctl     = ctl
        self.__length  = 0
        self.__options = options
        if options['verbose']: self.__memory = memory_usage()

        # set solving and restart policy
        self.__ctl.configuration.solve.solve_limit = "umax,"+str(options['restarts_per_solve'])
        if int(options['conflicts_per_restart']) != 0:
            self.__ctl.configuration.solver[0].restarts="F,"+str(options['conflicts_per_restart'])


    def __on_model(self,m):
        if self.__options['outf'] == 0:
            do_print("Answer: 1\n" + str(m))
        else:
            print "ANSWER\n" + " ".join([str(x)+"." for x in m.symbols(shown=True)])


    def __verbose_start(self):
        self.__time0 = clock()


    def __verbose_end(self,string):
        do_print(string+" Time:\t "+str(clock()-self.__time0),False)
        memory = memory_usage()
        do_print("Memory:\t\t "+str(memory)+"MB (+"+str(memory-self.__memory)+"MB)",False)
        self.__memory = memory


    def solve(self,length):

        global verbose_option

        do_print("Grounded Until:\t {}".format(self.__length),False)

        # ground if necessary
        if self.__length < length:
            parts = [(STEP,[t]) for t in range(self.__length+1,length+1)]
            parts = parts + [(CHECK,[length])]
            self.__ctl.release_external(clingo.Function(QUERY,[self.__length]))
            do_print("Grounding...\t "+str(parts),False)
            if verbose_option: self.__verbose_start()
            self.__ctl.ground(parts)
            if verbose_option: self.__verbose_end("Grounding")
            self.__ctl.assign_external(clingo.Function(QUERY,[length]),True)
            self.__ctl.cleanup()
            self.__length = length

        # no actions if necessary
        elif length < self.__length:
            do_print("Blocking actions...",False)
            for t in range(length+1,self.__length+1):
                self.__ctl.assign_external(clingo.Function(NO_ACTION,[t]),True)

        # solve
        do_print("Solving...")
        if verbose_option: self.__verbose_start()
        self.__result = self.__ctl.solve(on_model=self.__on_model)
        if verbose_option: self.__verbose_end("Solving")
        do_print(str(self.__result),False)

        # undo no actions if necessary
        if length < self.__length:
            do_print("Unblocking actions...",False)
            for t in range(length+1,self.__length+1):
                self.__ctl.assign_external(clingo.Function(NO_ACTION,[t]),False)

        # return
        do_print("",False)
        return self.__result


#
# PLANNER
#


class Planner:

    def run(self,options,clingo_options):

        ctl = clingo.Control(clingo_options)

        # input files
        for i in options['files']:
            ctl.load(i)
        if options['read_stdin']:
            ctl.add(BASE,[],sys.stdin.read())

        # additional programs
        ctl.add(BASE,[],PROGRAMS)

        # ground base, and set initial query
        ctl.ground([(BASE,[]),(CHECK,[0])])
        ctl.assign_external(clingo.Function(QUERY,[0]),True)

        # solver
        solver = Solver(ctl,options)

        # scheduler
        if sum([1 for i in ['A','B','C'] if options[i] is not None])>1: # check argument error
            raise Exception("Please, choose only one Scheduler: A, B, or C")
        if options['A'] is not None:  # (start,inc,limit,restarts,size,propagate_unsat)
            scheduler = A_Scheduler(options['start'],options['inc'],options['limit'],options['A'],options['propagate_unsat'])
        elif options['B'] is not None:
            scheduler = B_Scheduler(options['start'],options['inc'],options['limit'],options['processes'],options['propagate_unsat'],options['B'])
        elif options['C'] is not None:
            scheduler = A_Scheduler(options['start'],options['C'],  options['limit'],options['propagate_unsat'])
        else: # default
            scheduler = B_Scheduler(options['start'],options['inc'],options['limit'],options['processes'],options['propagate_unsat'],0.9)

        # loop
        i=1
        result = None
        global verbose_option
        if verbose_option: do_print("Memory: "+str(memory_usage())+"MB\n")
        while True:
            do_print("Iteration "+str(i),False)
            if verbose_option: time0 = clock()
            i += 1
            length = scheduler.next(result)
            if length == None:
                do_print("PLAN NOT FOUND")
                break
            result = solver.solve(length)
            if result.satisfiable:
                do_print("SATISFIABLE")
                break
            if verbose_option: do_print("Iteration Time:\t "+str(clock()-time0),False); do_print("",False)



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
planner.py -B 0.9 -M 20 -S 5 -F 0 -T 3000 -i 60 -r 100

planner is part of plasp in Potassco: https://potassco.org/
Get help/report bugs via : https://potassco.org/support
    """

    def run(self):

        # version
        _version = "planner.py version " + VERSION
        print _version

        # command parser
        _epilog = self.clingo_help + "\nusage: " + self.usage + self.epilog
        cmd_parser = argparse.ArgumentParser(description="An ASP Planner",
            usage=self.usage,epilog=_epilog,formatter_class=argparse.RawDescriptionHelpFormatter,
            add_help=False)

        # basic
        basic = cmd_parser.add_argument_group('Basic Options')
        basic.add_argument('-h','--help',action='help',help='Print help and exit')
        basic.add_argument('-',dest='read_stdin',action='store_true',help=argparse.SUPPRESS)
        basic.add_argument('-c','--const',dest='constants',action="append",help=argparse.SUPPRESS,default=[])
        basic.add_argument('-v','--verbose',dest='verbose',action="store_true",help="Be a bit more verbose")
        basic.add_argument('--outf',dest='outf',type=int,metavar="n",help="Use {0=default|1=competition} output",default=0,choices=[0,1])

        # Scheduler
        scheduler = cmd_parser.add_argument_group('Scheduler Options')

        # A, B or C
        scheduler.add_argument('-A',dest='A',help="Run algorithm A with parameter n (range 1 to 50)",
                                metavar='n',default=None,type=int)
        scheduler.add_argument('-B',dest='B',help="Run algorithm B with parameter r (range 0.1 to 0.9999) (default 0.9)",
                                metavar='r',default=None,type=float)
        scheduler.add_argument('-C',dest='C',help="Run algorithm C with parameter r (range 0.2 to 2.0)",
                                metavar='r',default=None,type=float)
        # Options
        scheduler.add_argument('-M',dest='processes',help="With algorithm B, use maximum n processes (default -M 20)",
                                metavar='n',default=20,type=int)
        scheduler.add_argument('-S',dest='inc',help="Step for horizon lengths 0, n, 2n, 3n, ... (default -S 5, algorithms A and B only)",
                                metavar='n',default=5,type=int)
        scheduler.add_argument('-F',dest='start',help="Starting horizon length (default -F 0)",
                                metavar='n',default=0,type=int)
        scheduler.add_argument('-T',dest='limit',help="Ending horizon length (default -T 3000)",
                                metavar='n',default=3000,type=int)
        scheduler.add_argument('-i',dest='conflicts_per_restart',help="Restart interval is n (default -i 60, use 0 for leaving open the restart policy)",
                                metavar='n',default=60,type=int)

        # New Options
        scheduler.add_argument('-r',dest='restarts_per_solve',help="Number of restarts per solve call (default -r 100)",
                                metavar='n',default=100,type=int)
        scheduler.add_argument('--keep-after-unsat',dest='propagate_unsat',help="After finding n to be UNSAT, do keep runs with m<n",
                                action="store_false")

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
        clingo_options.append(PLANNER_ON)

        # set printing options
        global verbose_option
        global outf
        verbose_option = options['verbose']
        outf           = options['outf']

        # return
        return options, clingo_options



#
# MAIN
#

if __name__ == "__main__":
    time0 = clock()
    options, clingo_options = PlannerArgumentParser().run()
    Planner().run(options,clingo_options)
    print "\nTime\t\t: {:.3f}s\n".format(clock() - time0)



