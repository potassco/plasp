# planner
> An ASP-based planner

## Usage
```bash
$ planner.py [options] [files]
```
Implements Algorithms A, B and C from [J. Rintanen](https://users.ics.aalto.fi/rintanen/jussi/satplan.html). 

Type ``--help`` for help.

## Description
The input consists of subprograms `base`, `check(t)` , and `step(t)`.
This is also the format of the clingo script [incmode-py.lp](https://github.com/potassco/clingo/blob/master/examples/clingo/iclingo/incmode-py.lp).

Let the program `P(n)` consist of subprograms `base`, `check` with `t=0..n`, and `step` with `t=1..n`.
Then the `planner` returns a stable model of the program consisting of `P(n)` and fact `query(n)`, 
for some `n>=0` such that the program is satisfiable.

The `planner`requires that for all `m>n`, `P(n)` with `query(n)` is satisfiable iff `P(m)` with `query(n)` is satisfiable.

## Additional predicates
The `planner` adds external predicates `query(t)` and `skip(t)`, 
which can only be used in the body of the rules.

While searching for a plan of length `n`, 
the `planner` sets both `query(n)`, and `skip(t)` for `t>n`, to `true`. 
The rest of the instances of those predicates are set to `false`.


## Solving Options

Option `--query-at-last` sets `query(m)` to true instead of `query(n)`, where `m` is the latest time point that the `planner` has grounded.

Option `--forbid-actions`  forbids actions at time points after current plan length `n`.
This uses predicate `occurs/2`, and is implemented by the following subprogram:
```bash
#program step(t).
:- occurs(A,t), skip(t).
```

Option `--force-actions`  forces at least one action at time points before current plan length `n`.
It is implemented by the following subprogram:
```bash
#program step(t).
:- not occurs(_,t), not skip(t).
```


## Examples
Replace in the Examples [here](https://github.com/potassco/plasp/blob/master/encodings/strips/README.md) `clingo incmode.lp` by `planner.py`.



# runplanner
> Running an ASP-based PDDL planner

## Usage
```bash
$ runplanner.py instance [options]
```

The path of the domain file may be given with option `--domain`.
Without this option, `runplanner.py` looks in the directory of the `instance` for a domain file 
named either `domain.pddl` or `domain_instance`. 

By default, `runplanner.py` uses `plasp` to translate the input to ASP facts, 
and `planner.py` for solving the planning problem with different encodings (see [here](https://github.com/potassco/plasp/blob/master/encodings/strips/README.md), and the available options with `--help`).
The `planner` is run with options `--query-at-last --forbid-actions --force-actions`.

With option `--translate`, it uses first `fast-downward` to translate the input to a [sas](http://www.fast-downward.org/TranslatorOutputFormat) file, 
then `plasp` to obtain ASP facts, and `planner.py`solves the problem with the mentioned encodings.

Option `--incmode` runs `clingo incmode.lp` instead of `planner.py`.

Option `--basic` applies `--translate` option but uses the [basic encoding](https://github.com/javier-romero/plasp/blob/master/encodings/planner/basic.lp).

Type ``--help`` for help.

### Internal Use Options 
With option `--fast-downward` it runs [fast-downward](http://www.fast-downward.org/) heuristic search planner with LAMA settings, 
and with options `--M`, `--Mp`, and `--MpC`, it runs different versions of the [madagascar](https://users.ics.aalto.fi/rintanen/jussi/satplan.html) SAT planner.


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

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --incmode

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --basic

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --basic --postprocess

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --fast-downward

runplanner.py ../../instances/PDDL/ipc-2000-elevator-m10-strips/problem-04-00.pddl --madagascar
```

