# Output Format

`plasp` 3 translates SAS and PDDL files into the same ASP fact format.

## Sections

The `plasp` output contains the following sections:

section | description
--------|------------
feature requirements | advanced features (such as conditional effects or axiom rules) required by the input planning problem
types (PDDL only) | object types used by the planning problem (only if typing is enabled)
variables | fluents used by the planning problem (variables in SAS and predicates in PDDL) and their possible values
actions | actions (operators in SAS and actions in PDDL), their preconditions, and effects
mutex groups (SAS only) | sets of mutually exclusive variable assignments
axiom rules (SAS only) | special actions that are immediately executed when the preconditions are satisfied
objects (PDDL only) | constant objects defined by PDDL domains or problems
initial state | initial variable assignment
goal | target variable assignment
