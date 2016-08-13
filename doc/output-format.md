# Output Format

`plasp` 3 translates SAS and PDDL files into the same ASP fact format.

## Syntax and Semantics

`plasp` structures the translated ASP facts into the multiple sections, which are explained in the following.

### Feature Requirements

Declares advanced features required by the input planning problem, such as conditional effects and axiom rules.

Feature requirements may be used in meta encodings to warn about features that are unsupported by the encoding.

syntax | description
-------|------------
`requiresFeature(<feature>).` | declares the feature `<feature>` to be required by the input problem

Currently, feature requirements are only recognized with SAS problems.

SAS feature | description
------------|------------
`actionCosts` | actions have associated costs
`axiomRules` | immediate actions used, which are executed as soon as the preconditions are satisfied
`conditionalEffects` | some effects of an action may have additional conditions
