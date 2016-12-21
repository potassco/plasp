# planner
> An ASP-based planner

## Usage
```bash
$ planner.py [options] [files]
```
Implements Algorithms S, A, B and C from [J. Rintanen](https://users.ics.aalto.fi/rintanen/jussi/satplan.html). 

Type ``--help`` for help.

## Requirements
The input must be in the format accepted by the clingo script [incmode-py.lp](https://github.com/potassco/clingo/blob/master/examples/clingo/iclingo/incmode-py.lp).

The planner relies on predicate `occurs(action,t)` representing action occurrences.

It adds an external predicate `no_action(t)`, which should not appear in the input encodings.

The planner sets constant `planner_on` to value `true`.

The input encodings must allow for the non execution of actions at any time point.
However, for using the encoding without the planner, 
this can be circumvented as follows with constant `planner_on`:
```bash
#const planner_on=false.
:- not 1 { occurs(A,t) }, planner_on=false.
```

## Examples
Replace in the Examples [here](https://github.com/potassco/plasp/blob/master/encodings/strips/README.md) `clingo incmode-py.lp` by `planner.py`




# runplanner
> Running an ASP-based PDDL planner

## Usage
```bash
$ runplanner.py instance [options]
```

`runplanner.py` uses `plasp` to translate the input to ASP facts, 
and `planner.py` for solving the planning problem with different encodings (see [here](https://github.com/potassco/plasp/blob/master/encodings/strips/README.md), and the available options with `--help`).

With option `--translate`, it uses first `fast-downward` to translate the input to a [sas](http://www.fast-downward.org/TranslatorOutputFormat) file, 
then `plasp` to obtain ASP facts, and `planner.py`solves the problem with the mentioned encodings.

Option `--basic` applies `--translate` option but uses the [basic encoding](https://github.com/javier-romero/plasp/blob/master/encodings/planner/basic.lp).

With option `--fast-downward` it runs [fast-downward](http://www.fast-downward.org/) heuristic search planner with LAMA settings, 
and with option `--madagascar` it runs [madagascar](https://users.ics.aalto.fi/rintanen/jussi/satplan.html) SAT planner.

Type ``--help`` for help.

## Requirements
If there is a domain file, it should be named `domain.pddl` and appear in the same directory as the `instance`.

## Examples
Some Example invocations (modifying [these](https://github.com/potassco/plasp/blob/master/encodings/strips/README.md)):
```bash
runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl 

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --closure=0

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --closure=1

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --closure=2

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --parallel=1

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --parallel=2

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --parallel=1 --redundancy

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --parallel=2 --redundancy

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --postprocess

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --parallel=1 --postprocess

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --parallel=2 --postprocess

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --translate

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --closure=0 --translate

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --closure=1 --translate

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --closure=2 --translate

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --parallel=1 --translate

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --parallel=2 --translate

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --parallel=1 --redundancy --translate

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --parallel=2 --redundancy --translate

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl  --translate --postprocess

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --parallel=1 --translate --postprocess

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --parallel=2 --translate --postprocess

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --basic

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --basic --postprocess

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --fast-downward

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --madagascar
```

