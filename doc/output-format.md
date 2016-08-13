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
