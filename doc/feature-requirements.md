# Feature Requirements

Feature requirements are part of `plasp`’s [output format](output-format.md).
Currently, `plasp` detects feature requirements only for SAS problems.

`plasp` supports the following feature requirements:

SAS feature | description
------------|------------
`actionCosts` | actions have associated costs (see [action costs](output-format.md#action-costs))
`axiomRules` | immediate actions are used, which are executed as soon as the preconditions are satisfied (see [axiom rules](output-format.md#axiom-rules))
`conditionalEffects` | some effects of an action may have additional conditions (see [actions](output-format.md#actions))

## Example

The following specifies that the input problem has the two requirements `actionCosts` and `conditionalEffects`.

```prolog
% requirements
requiresFeature(actionCosts).
requiresFeature(conditionalEffects).
```
