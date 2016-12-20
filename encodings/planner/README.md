# planner
> An ASP-based planner

## Usage
```bash
$ planner.py [options] [files]
```
Implements Algorithms S, A, B and C from [J. Rintanen](https://users.ics.aalto.fi/rintanen/jussi/satplan.html). Type ``--help`` for help.

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
:- not 1 { occurs(A,t) : active(A) }, planner_on=false.
```

## Examples
Replace in the Examples [here](https://github.com/potassco/plasp/blob/master/encodings/strips/README.md) `clingo incmode-py.lp` by `planner.py`


# runplanner
> Running an ASP-based PDDL planner

## Usage
```bash
$ planner.py [options] [files]
```

Implements Algorithms S, A, B and C from [J. Rintanen](https://users.ics.aalto.fi/rintanen/jussi/satplan.html). Type ``--help`` for help.

## Examples
Replace in the Examples [here](https://github.com/potassco/plasp/blob/master/encodings/strips/README.md) `clingo incmode-py.lp` by `planner.py`
