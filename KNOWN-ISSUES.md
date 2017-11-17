# Known Issues

## Parser

- multiple PDDL features are not yet supported (see list of PDDL features supported by `plasp`)
- several error messages obtained with syntax errors are misleading

## Translator

- in compatibility mode, the outdated `vars` sections within action definitions are handled just like `parameters`, which is semantically incorrect
- equality expressions currently partly supported by parser but not the translator

## ASP Output Format

- SAS axiom rules and `plasp`’s derived predicates (obtained through normalization) are semantically similar, but differently represented

## Encodings

- the [simple example encoding](encodings/sequential-horizon.lp) does not SAS axiom rules
- [multiple encodings](encodings/outdated) are outdated and need to be adapted to the output format introduced with `plasp` 3.1.0
