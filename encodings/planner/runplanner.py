#!/usr/bin/python
import sys
import os
import argparse

PLASP       = "plasp"
PLASP_DIR   = os.path.dirname(os.path.realpath(__file__)) + "/../../"
PLANNER     = PLASP_DIR + "encodings/planner/planner.py"
BASIC       = PLASP_DIR + "encodings/planner/basic.lp"
PREPROCESS  = PLASP_DIR + "encodings/strips/preprocess.lp"
STRIPS      = PLASP_DIR + "encodings/strips/strips-incremental.lp"
REDUNDANCY  = PLASP_DIR + "encodings/strips/redundancy.lp"
POSTPROCESS = PLASP_DIR + "encodings/strips/postprocess.lp"
INCMODE     = PLASP_DIR + "encodings/strips/incmode.lp"
TMP         = os.path.dirname(os.path.realpath(__file__)) + "/run.tmp"

# Other systems
CLINGO      = "clingo"
FAST_D      = "/home/wv/bin/linux/64/fast-downward-data/fast-downward.py --alias seq-sat-lama-2011"
FAST_D_TR   = "/home/wv/bin/linux/64/fast-downward-data/fast-downward.py --translate"
SAS_OUTPUT  = "output.sas"
M           = "M"
MP          = "Mp"
MPC         = "MpC"



class MyArgumentParser:

    help = """
Planner and Clingo Options:
  --<option>[=<value>]\tSet planner or clingo <option> [to <value>]

    """
    usage = "runplanner.py instance [options]"
    epilog = " "
    epilog = """
Default command-line:
runplanner.py instance --closure=3 --parallel=0

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
        basic.add_argument('instance',help="PDDL instance, with corresponding domain file in the same directory (named either domain.pddl, or domain_instance)")

        # specific
        normal = cmd_parser.add_argument_group('Solving Options')
        normal.add_argument('--translate','-tr',dest='translate',action='store_true',help='Run fast-downward translator to sas, then plasp translator, and solve')
        normal.add_argument('--closure',default=3,type=int,choices=[0,1,2,3],help='Static analysis of potentially relevant actions (default: 3)')
        normal.add_argument('--parallel',default=0,type=int,choices=[0,1,2],help='Sequential and parallel planning encoding variants (default: 0)')
        normal.add_argument('--redundancy',action='store_true',help='Enforcement of redundant actions')
        normal.add_argument('--postprocess',action='store_true',help='Solve, serialize, and check if solution is correct (works also with --basic)')


        extended = cmd_parser.add_argument_group('Other Solving Modes')
        extended.add_argument('--incmode',dest='incmode',action='store_true',help='Run clingo incmode')
        extended.add_argument('--basic',dest='basic',action='store_true',help='Run fast-downward translator to sas, then plasp translator, and solve with the basic encoding')
        extended.add_argument('--fast-downward','-fd',dest='fast-downward',action='store_true',help='Run fast-downward heuristic search planner with LAMA settings')
        extended.add_argument('--madagascar-M',  dest='M',  action='store_true',help='Run version   M of madagascar SAT planner')
        extended.add_argument('--madagascar-Mp', dest='Mp', action='store_true',help='Run version  Mp of madagascar SAT planner')
        extended.add_argument('--madagascar-MpC',dest='MpC',action='store_true',help='Run version MpC of madagascar SAT planner')

        # parse
        options, unknown = cmd_parser.parse_known_args()
        options = vars(options)

        # check
        if options['redundancy'] and options['parallel']==0:
            raise Exception('command error: redundancy option must be always issued together with parallel option 1 or 2')
        # return
        return options, unknown


def run():

    options, rest = MyArgumentParser().run()
    instance = options['instance']
    domain   = os.path.dirname(os.path.realpath(instance)) + "/domain.pddl"
    if not os.path.isfile(domain):
        domain = os.path.dirname(os.path.realpath(instance)) + "/domain_" + os.path.basename(instance)

    #
    # NORMAL CASE
    #

    # translate to facts
    if options['translate']:
        call = "{} {} {}; {} {}".format(FAST_D_TR,domain,instance,PLASP,SAS_OUTPUT)
    else:
        call = "{} {} {}".format(PLASP,domain,instance)

    # postprocess
    postprocess = ""
    if options['postprocess']:
        if options['translate'] or options['basic']:
            postprocess = " --outf=1 | grep -A1 ANSWER | tail -n1 > {}; {} {}    | clingo - {} {}; rm {}".format(TMP,PLASP,     SAS_OUTPUT,POSTPROCESS,TMP,TMP)
        else:
            postprocess = " --outf=1 | grep -A1 ANSWER | tail -n1 > {}; {} {} {} | clingo - {} {}; rm {}".format(TMP,PLASP,instance,domain,POSTPROCESS,TMP,TMP)

    # normal plan
    call += " | {} - {} {} {}".format(PLANNER,PREPROCESS,STRIPS,
        (" ".join(rest))                                           +
        " -c _closure={}  ".format(options['closure'])             +
        " -c _parallel={} ".format(options['parallel'])            +
        (" " + REDUNDANCY + " " if options['redundancy']  else "") +
        postprocess)

    #
    # OTHER CASES
    #

    # basic encoding
    if options['incmode']:
        call = call.replace(PLANNER,CLINGO + " " + INCMODE)
    elif options['basic']:
        call = "{} {} {}; {} {} | {} - {} {}".format(FAST_D_TR,domain,instance,PLASP,SAS_OUTPUT,PLANNER,BASIC," ".join(rest) +
               (postprocess if options['postprocess'] else ""))
    # fast-downward
    elif options['fast-downward']:
        call = "{} {} {} {}".format(FAST_D,domain,instance," ".join(rest))
    # madagascar
    elif options['M']:
        call = "{} {} {} {}".format(  M,domain,instance," ".join(rest))
    elif options['Mp']:
        call = "{} {} {} {}".format( MP,domain,instance," ".join(rest))
    elif options['MpC']:
        call = "{} {} {} {}".format(MPC,domain,instance," ".join(rest))

    #
    # SOLVE
    #

    if options['print']:
        print call
    else:
        os.system(call)

run()
