#!/usr/bin/python
import sys
import os
import argparse

PLASP       = os.path.dirname(os.path.realpath(__file__)) + "/../../"
PLANNER     = PLASP + "encodings/planner/planner.py"
PREPROCESS  = PLASP + "encodings/strips/preprocess.lp"
STRIPS      = PLASP + "encodings/strips/strips-incremental.lp"
REDUNDANCY  = PLASP + "encodings/strips/redundancy.lp"
POSTPROCESS = PLASP + "encodings/strips/postprocess.lp"
TMP         = os.path.dirname(os.path.realpath(__file__)) + "/run.tmp"
FAST_D      = "/home/wv/bin/linux/64/fast-downward-data/fast-downward.py --alias seq-sat-lama-2011"
MADAGASCAR  = "MpC"

class MyArgumentParser:

    help = """
Planner and Clingo Options:
  --<option>[=<value>]\tSet planner or clingo <option> [to <value>]

    """
    usage = "runplanner.py instance [options] [files]"
    epilog = " "
    epilog = """
Default command-line:
runplanner.py

runplanner.py is part of Potassco: https://potassco.org/labs
Get help/report bugs via : https://potassco.org/support
    """

    def run(self):

        # command parser
        _epilog = self.help + "\nusage: " + self.usage + self.epilog
        cmd_parser = argparse.ArgumentParser(description='Running an ASP-based PDDL Planner',
            usage=self.usage,epilog=_epilog,formatter_class=argparse.RawDescriptionHelpFormatter, add_help=False)

        # basic
        basic = cmd_parser.add_argument_group('Basic Options')
        basic.add_argument('-h','--help',action='help',help='Print help and exit')
        basic.add_argument('-p','--print',dest='print',action='store_true',help='Print system call and exit')
        #basic.add_argument('-',dest='read_stdin',action='store_true',help=argparse.SUPPRESS)
        #basic.add_argument('-c','--const',dest='constants',action="append",help=argparse.SUPPRESS,default=[])
        #basic.add_argument('-v','--verbose',dest='verbose',action="store_true",help="Be a bit more verbose")

        # specific
        specific = cmd_parser.add_argument_group('Solving Options')
        specific.add_argument('instance',help="PDDL instance, with corresponding domain.pddl in the same directory")
        specific.add_argument('--closure',default=3,type=int,choices=[0,1,2,3],help='Static analysis of potentially relevant actions (default: 3)')
        specific.add_argument('--parallel',default=None,type=int,choices=[0,1,2],help='Sequential and parallel planning encoding variants (default: 0)')
        specific.add_argument('--redundancy',action='store_true',help='Enforcement of redundant actions')
        specific.add_argument('--postprocess',action='store_true',help='Solve, serialize, and check if solution is correct')
        specific.add_argument('--fast-downward','-fd',dest='fast-downward',action='store_true',help='Run fast-downward heuristic search planner with LAMA settings')
        specific.add_argument('--madagascar','-m',dest='madagascar',action='store_true',help='Run madagascar SAT planner')

        # parse
        options, unknown = cmd_parser.parse_known_args()
        options = vars(options)

        # check
        if options['redundancy'] and options['parallel'] is not None:
            raise Exception('command error: redundancy option must be always issued together with parallel option')
        if options['fast-downward'] and options['madagascar']:
            raise Exception('command error: i can plan, but only with one planner at a time, dear ;)')

        # return
        return options, unknown


def run():
    options, rest = MyArgumentParser().run()
    instance = options['instance']
    domain   = os.path.dirname(os.path.realpath(instance)) + "/domain.pddl"
    postprocess = " --outf=1 | grep -A1 ANSWER | tail -n1 > {}; plasp {} {} | clingo - {} {}; rm {}".format(TMP,instance,domain,POSTPROCESS,TMP,TMP)
    call = "plasp {} {} | {} - {} {} {}".format(instance,domain,PLANNER,PREPROCESS,STRIPS,
        (" ".join(rest))                                                                             +
        (" -c _closure={} ".format(options['closure'])   if options['closure']  is not None else "") +
        (" -c _parallel={} ".format(options['parallel']) if options['parallel'] is not None else "") +
        (" " + REDUNDANCY + " " if options['redundancy'] is not None else "")                        +
        (postprocess            if options['postprocess']            else ""))
    if options['fast-downward']:
        call = "{} {} {} {}".format(FAST_D,domain,instance," ".join(rest))
    elif options['madagascar']:
        call = "{} {} {} {}".format(MADAGASCAR,domain,instance," ".join(rest))
    if options['print']:
        print call
    else:
        os.system(call)

run()
