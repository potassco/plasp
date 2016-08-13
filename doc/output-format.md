# Output Format

`plasp` 3 translates SAS and PDDL files into the same ASP fact format.

## In a Nutshell

The following illustrates `plasp`’s output format for the problem of turning switches on and off.

```prolog
% declares the type "type(switch)"
type(type(switch)).

% introduces a switch "constant(a)"
constant(constant(a)).
has(constant(a), type(switch)).

% introduces another switch "constant(a)"
constant(constant(b)).
has(constant(b), type(switch)).

% declares a variable "variable(on(X))" for each switch X
variable(variable(on(X))) :- has(X, type(switch)).

% the variable may be true or false
contains(variable(on(X)), value(on(X)), true)) :- has(X, type(switch)).
contains(variable(on(X)), value(on(X)), false)) :- has(X, type(switch)).

% declares the action "action(turnOn(X))", which requires switch X to be off and then turns it on
action(action(turnOn(X))) :- has(X, type(switch)).
precondition(action(turnOn(X)), variable(on(X)), value(on(X), false)) :- has(X, type(switch)).
postcondition(action(turnOn(X)), effect(0), variable(on(X)), value(on(X), true)) :- has(X, type(switch)).

% declares the action "action(turnOff(X))", which requires switch X to be on and then turns it off
action(action(turnOff(X))) :- has(X, type(switch)).
precondition(action(turnOff(X)), variable(on(X)), value(on(X), true)) :- has(X, type(switch)).
postcondition(action(turnOff(X)), effect(0), variable(on(X)), value(on(X), false)) :- has(X, type(switch)).

% initially, switch a is off and switch b is on
initialState(variable(on(constant(a))), value(on(constant(a)), false)).
initialState(variable(on(constant(b))), value(on(constant(b)), true)).

% in the end, switch a should be on and switch b should be off
goal(variable(on(constant(a))), value(on(constant(a)), true)).
goal(variable(on(constant(b))), value(on(constant(b)), false)).
```

When translating SAS or PDDL problems, `plasp` structures the translated ASP facts into multiple sections, which are explained in the following.

## Feature Requirements

Declares advanced features required by the input planning problem, such as conditional effects and axiom rules.

Feature requirements may be used in meta encodings to warn about unsupported features.

### Syntax

syntax | description
-------|------------
`requiresFeature(<feature name>).` | declares the feature `<feature name>` to be required by the input problem

Currently, feature requirements are only recognized with SAS problems.
`plasp` supports the following feature requirements:

SAS feature | description
------------|------------
`actionCosts` | actions have associated costs
`axiomRules` | immediate actions are used, which are executed as soon as the preconditions are satisfied
`conditionalEffects` | some effects of an action may have additional conditions

### Example

The following specifies that the input problem has the two requirements `actionCosts` and `conditionalEffects`.

```prolog
% requirements
requiresFeature(actionCosts).
requiresFeature(conditionalEffects).
```

## Types (PDDL Only)

Specifies all object types used by the PDDL problem (only if typing is enabled).

### Syntax

syntax | description
-------|------------
`type(type(<type name>)).` | declares the type `type(<type name>)`
`has(<object>, type(<type name>)).` | declares `<object>` to be of type `type(<type name>)`

### Example

The following declares the type `type(block)` for later usage by the problem:

```prolog
% types
type(type(block)).
```

[Constants](#constants) may have associated types (here, `constant(a)`):

```prolog
constant(constant(a)).
has(constant(a), type(block)).
```

If there is typing information, [variables](#variables) are derived only for the allowed types (here, `variable(clear(X))` with `type(block)`):

```prolog
variable(variable(clear(X))) :- has(X, type(block)).
```

Similarly, [actions](#actions) are derived in a type-safe way (here, `action(stack(X, Y))` is only applicable to arguments of `type(block)`):

```prolog
action(action(stack(X, Y))) :- has(X, type(block)), has(Y, type(block)).
```
