# Output Format

`plasp` 3 translates SAS and PDDL files into the same ASP fact format.

`plasp` structures the translated ASP facts into the multiple sections, which are explained in the following.

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
