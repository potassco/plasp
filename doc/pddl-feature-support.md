# PDDL Feature Support

This document describes the PDDL features are currently supported by `plasp`’s very own translator.

This doesn’t cover PDDL feature support through SAS conversion with [Fast Downward](http://www.fast-downward.org/) (please refer to [Fast Downward’s homepage](http://www.fast-downward.org/) for more information).

The table below lists the features of PDDL 3.1 and whether or not they are supported by `plasp`.
Unless otherwise noted, this refers to `plasp`’s parser, normalizer, translator, and ASP output format.

| PDDL feature | description | supported by `plasp` |
|---|---|---|
| STRIPS | basic STRIPS functionality, the general PDDL sections (`:domain`, `:problem`, `:predicates`, `:action`) | yes |
| typing | typed variables and constants, type inheritance | yes (except for `either` type directives) |
| negative preconditions | typed variables and constants, type inheritance | yes |
| disjunctive preconditions | `or` expressions in preconditions | yes |
| existential preconditions | `exists` expressions in preconditions | yes |
| universal preconditions | `forall` expressions in preconditions | yes |
| conditional effects | `when` expressions in action effects | yes |
| universal effects | `forall` expressions in effects | yes |
| equality | `=` expressions for atomic formulas | no (parser support only) |
| numeric fluents | numeric function definitions in `:functions` section, assignment operators, arithmetic preconditions  | no |
| action costs | actions are assigned costs through numeric fluents | no |
| durative actions | temporal planning with action durations | no |
| derived predicates | predicates whose truth value is defined by a formula | no (`plasp` introduces *custom* derived predicates to support nested expressions, but derived predicates specified in the PDDL input are currently not supported) |
| timed initial literals | initial state may specify literals that become true at a specified time point | no |
| preferences | preferences in action preconditions and goals | no |
| constraints | additional constraints for domain and problem | no |
