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
outf           = 0
def do_print(string,verbose=True):
    if verbose or verbose_option:
        print ("% " if outf==1 else "") + string


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


    def next(self,result):
        if result == None:
            self.__runs = [(self.__length,self.__time)]
            return self.__runs[0][0], self.__runs[0][1]
        if result.interrupted: # or result.satisfiable:
            return self.__runs[0][0], self.__runs[0][1]
        self.__length = self.__length + self.__inc
        self.__runs = [(self.__length,self.__time)]
        return self.__runs[0][0], self.__runs[0][1]



class A_Scheduler(Scheduler):


    def __init__(self,start,inc,time,queue):
        self.__length = start
        self.__inc    = inc
        self.__time   = time
        self.__queue  = queue
        self.__runs   = []


    def next(self,result):
        if result == None:     # add all runs
            self.__runs   = [ (self.__length + (i*self.__inc), self.__time) for i in range(self.__queue)]
            self.__length = self.__length + ((self.__queue-1)*self.__inc)
        elif result.interrupted: # or result.satisfiable: # move to the end of the queue
            self.__runs.append(self.__runs[0])
            self.__runs = self.__runs[1:]
        else:
            self.__length = self.__length + self.__inc
            self.__runs.append((self.__length,self.__time))
            self.__runs = self.__runs[1:]
        do_print("Queue:\t\t " + str(self.__runs),False)
        return self.__runs[0][0], self.__runs[0][1]



class B_Scheduler:


    def __init__(self,n_start,n_inc,t_start,t_inc,gamma):
        self.__n_next = n_start
        self.__n_inc  = n_inc
        self.__t      = t_start
        self.__t_inc  = t_inc
        self.__gamma  = gamma
        self.__runs     = []
        self.__unsolved = []
        self.__step     = 0


    def next(self,result):
        # result
        if result is not None:
            if result.interrupted:
                self.__unsolved.append(self.__runs[0])
            self.__runs = self.__runs[1:]
        # if no more runs
        if self.__runs == []:
            # unsolved
            tmp = []
            for i in self.__unsolved:
                t = int(self.__t * (self.__gamma ** i[3]))
                if t > i[2]:
                    # (planning time point, time for next run, total time, step)
                    self.__runs.append((i[0],t-i[2],t,i[3]))
                else:
                    tmp.append(i)
            self.__unsolved = tmp
            # next ones
            n, step = self.__n_next, self.__step
            while True:
                t = int(self.__t * (self.__gamma ** step))
                if t > 0:
                    # (planning time point, time for next run, total time, step)
                    self.__runs.append((n,t,t,step))
                    n    += self.__n_inc
                    step += 1
                else:
                    self.__t += self.__t_inc
                    if len(self.__runs)>0:
                        self.__n_next, self.__step = n, step
                        break
        # print
        do_print("Queue:\t\t " + str(self.__runs),False)
        # return
        return self.__runs[0][0], self.__runs[0][1]


class C_Scheduler(Scheduler):


    def __init__(self,time,queue):
        self.__length = 1
        self.__inc    = 2
        self.__time   = time
        self.__queue  = queue
        self.__runs   = []


    def next(self,result):
        if result == None:     # add all runs
            self.__runs   = [ (self.__inc ** i, self.__time) for i in range(self.__queue)]
            self.__length =    self.__inc ** (self.__queue-1)
        elif result.interrupted: # move to the end of the queue
            self.__runs.append(self.__runs[0])
            self.__runs = self.__runs[1:]
        else:
            self.__length = self.__length * self.__inc
            self.__runs.append((self.__length,self.__time))
            self.__runs = self.__runs[1:]
        do_print("Queue:\t\t " + str(self.__runs),False)
        return self.__runs[0][0], self.__runs[0][1]




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


    def __on_model(self,m):
        if self.__options['outf'] == 0:
            do_print("Answer: 1\n" + str(m))
        else:
            print "ANSWER\n" + " ".join([str(x)+"." for x in m.symbols(shown=True)])


    def __on_finish(self,result):
        self.__result = result


    def solve(self,length,time):

        do_print("Grounded Until:\t {}".format(self.__length),False)

        # ground if necessary
        if self.__length < length:
            parts = [(STEP,[t]) for t in range(self.__length+1,length+1)]
            parts = parts + [(CHECK,[length])]
            self.__ctl.release_external(clingo.Function(QUERY,[self.__length]))
            do_print("Grounding...\t "+str(parts),False)
            self.__ctl.ground(parts)
            self.__ctl.assign_external(clingo.Function(QUERY,[length]),True)
            self.__ctl.cleanup()
            self.__length = length

        # no actions if necessary
        elif length < self.__length:
            do_print("Blocking actions...",False)
            for t in range(length+1,self.__length+1):
                self.__ctl.assign_external(clingo.Function(NO_ACTION,[t]),True)

        # solve with time limit
        do_print("Solving...")
        future = self.__ctl.solve_async(on_model=self.__on_model,on_finish=self.__on_finish) # sets self.__result
        future.wait(time) or future.cancel()
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

        # initialize
        ctl.ground([(BASE,[])])
        scheduler = B_Scheduler(5,5,5,5,0.8) # start,inc,time
        if options['S'] is not None:
            s = [int(x) for x in options['S']]
            scheduler = Sequential_Scheduler(s[0],s[1],s[2])      # start,inc,time
        elif options['A'] is not None:
            a = [int(x) for x in options['A']]
            scheduler = A_Scheduler(a[0],a[1],a[2],a[3]) # start,inc,time,queue
        elif options['B'] is not None:
            a = [int(x) for x in options['B'][:-1]]
            scheduler = B_Scheduler(a[0],a[1],a[2],a[3],float(options['B'][-1])) # start,inc,time,queue
        elif options['C'] is not None:
            a = [int(x) for x in options['C']]
            scheduler = C_Scheduler(a[0],a[1]) # queue
        solver    = Solver(ctl,options)

        # loop
        i=1
        result = None
        while True:
            do_print("Iteration "+str(i),False)
            i += 1
            length, time = scheduler.next(result)
            result       = solver.solve(length,time)
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
planner.py -B 5 5 1 1 0.9

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
        basic.add_argument('--outf',dest='outf',type=int,metavar="<n>",help="Use {0=default|1=competition} output",default=0,choices=[0,1])
        # scheduler
        scheduler = cmd_parser.add_argument_group('Scheduler Options')
        scheduler.add_argument('-S',nargs=3,dest='S',help="Sequential Scheduler starting at time point <s>,\
                               incrementing <i> time points, and solving for <t> seconds",metavar=('<s>','<i>','<t>'),default=None)
        scheduler.add_argument('-A',nargs=4,dest='A',help="Scheduler A starting at time point <s>, incrementing <i> time points,\
                                solving for <t> seconds with a queue of <q> elements",
                                metavar=('<s>','<i>','<t>','<q>'),default=None)
        scheduler.add_argument('-B',nargs=5,dest='B',help="Scheduler B starting at time point <s>, incrementing <i> time points,\
                                solving initially for <t> seconds, incrementing <it> seconds,\
                                with gamma <g> (float)",
                                metavar=('<s>','<i>','<t>','<it>','<g>'),default=None)
        scheduler.add_argument('-C',nargs=2,dest='C',help="Scheduler C, solving for <t> seconds, with a queue of <q> elements",
                                metavar=('<t>','<q>'),default=None)

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
    options, clingo_options = PlannerArgumentParser().run()
    Planner().run(options,clingo_options)

