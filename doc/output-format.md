# Output Format

`plasp` 3 translates SAS and PDDL files into a uniform ASP fact format.

## Overview

Essentially, `plasp`’s output format consists of [state variables](#variables) that are modified by [actions](#actions) if their preconditions are fulfilled.
Variables reference [entities](#constants-objects) that are affected by the actions.
As with PDDL, the objective is to achieve a specific [goal](#goal) starting from an [initial state](#initial-state) by executing a sequence of actions.

`plasp`’s variables correspond to the multivalued variables in SAS.
PDDL predicates are turned into Boolean variables to make the output format consistent.

Actions are modeled exactly as PDDL actions and SAS operators.

## In a Nutshell

The following illustrates `plasp`’s output format for the problem of turning switches on and off.

```prolog
% declares the type "type(switch)"
type(type(switch)).

% introduces a switch "constant(a)"
constant(constant(a)).
has(constant(a), type(switch)).

% declares a variable "variable(on(X))" for switches X
variable(variable(on(X))) :- has(X, type(switch)).

% the variable may be true or false
contains(variable(on(X)), value(on(X)), true)) :- has(X, type(switch)).
contains(variable(on(X)), value(on(X)), false)) :- has(X, type(switch)).

% declares the action "action(turnOn(X))", which requires switch X to be off and then turns it on
action(action(turnOn(X))) :- has(X, type(switch)).
precondition(action(turnOn(X)), variable(on(X)), value(on(X), false)) :- has(X, type(switch)).
postcondition(action(turnOn(X)), effect(0), variable(on(X)), value(on(X), true)) :- has(X, type(switch)).

% initially, the switch is off
initialState(variable(on(constant(a))), value(on(constant(a)), false)).

% in the end, the switch should be on
goal(variable(on(constant(a))), value(on(constant(a)), true)).
```

## Syntax and Semantics

`plasp` structures the translated ASP facts into multiple sections, which are explained in the following.

### Feature Requirements

```prolog
% declares a required feature
requires(feature(<name>)).
```

`plasp` recognizes and declares advanced features used by the input problem, such as conditional effects and [axiom rules](#axiom-rules) (currently only SAS).
See the [full list of supported features](feature-requirements.md) for more information.

The feature requirement predicates may be used in meta encodings to warn about unsupported features.

### Types

```prolog
% declares a <type>
type(type(<name>)).

% specifies <constant> to be of type type(<name>)
has(<constant>, type(<name>)).
```

[Variables](#variables), [constants](#constants-objects), and [objects](#constants-objects) may be typed. Types are only available with PDDL and if typing is enabled.

### Variables

```prolog
% declares a <variable>
variable(variable(<name>)).

% adds a <value> to the domain of a <variable>
contains(<variable>, <value>).
```

`plasp`’s variables represent the current state of the planning problem.
Variables are linked to the problem's [objects](#constants-objects) and [constants](#constants-objects).

`plasp`’s variables are multivalued, and each variable has exactly one value at each point in time.

With SAS, variable names are numbers starting at 0, `variable(<number>)`.
SAS variables are inherently multivalued, which results in two or more values of the form `value(<SAS predicate>, <bool>)` for each variable.

With PDDL, Boolean variables are created from the PDDL predicates.
Variables ared named after the PDDL predicates, `variable(<PDDL predicate>).`
Each variable contains exactly two values (one `true`, one `false`) of the form `value(<PDDL predicate>, <bool>)`.
Note that with PDDL, variables and values are named identically.

### Actions

```prolog
% declares an <action>
action(action(<name>)).

% defines that as a precondition to <action>, <variable> must have value <value>
precondition(<action>, <variable>, <value>).

% defines that after executing <action>, <variable> is assigned <value>
postcondition(<action>, effect(<number>), <variable>, <value>).

% defines the condition of a conditional effect
precondition(effect(<number>), <variable>, <value>).

% specifies the costs of applying <action>
costs(<action>, <number>).
```

Actions may require certain variables to have specific values in order to be executed.
After applying an action, variables get new values according to the action's postconditions.

Actions may have *conditional effects*, that is, certain postconditions are only applied if additional conditions are satisfied.
For this reason, each conditional effect is uniquely identified with a predicate `effect(<number>)` as the second argument of the `postcondition` facts.
The conditions of conditional effects are given by additional `precondition` facts that take the respective `effect(<number>)` predicates as the first argument.

Unconditional effects are identified with `effect(unconditional)`.

Conditional effects are currently only supported with SAS input problems.

Actions may also have *action costs* required to apply them. Action costs are currently supported for SAS only.

### Constants/Objects

```prolog
% declares a <constant> or object
constant(constant(<name>)).

% specifies <constant> to be of type type(<name>)
has(<constant>, <type>).
```

Constants and objects are the entities that are affected by [actions](#actions), for instance, the blocks in a Blocks World problem.
Constants are global for a domain, while objects are problem-specific.

`plasp` does not distinguish between the two (modeling both as constants), as both are identically used static identifiers.

### Initial State

```prolog
% initializes <variable> with a specific <value>
initialState(<variable>, <value>).
```

The initial state contains all variable assignments that hold before executing any actions.

Note that with PDDL, the initial state might not assign values to all variables. Instead, unassigned values have to be assigned `false` manually.

### Goal

```prolog
% specifies that <variable> shall obtain <value> in the end
goal(<variable>, <value>).
```

The goal specifies all variable assignments that have to be fulfilled after executing the plan.
