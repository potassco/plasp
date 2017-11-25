# Changelog

## 3.1.1 (2017-11-25)

### Bug Fixes

* fixes ASP syntax issues caused by hyphens in variable names by replacing all user-defined variables with `X1`, `X2`, etc.

## 3.1.0 (2017-11-17)

### Changes

* new command-line interface based on `plasp` commands:
  * `plasp translate`: translate PDDL and SAS to ASP facts (replaces former, plain `plasp` calls)
  * `plasp normalize`: normalize PDDL to plasp’s custom PDDL format
  * `plasp check-syntax`: check the syntax of PDDL specifications
  * `plasp beautify`: cleanly format PDDL specifications
* new output format to accommodate for new PDDL features

### Features

* extended PDDL 3.1 support:
  * arbitrarily nested expressions in preconditions, effects, and goal description (via derived predicates)
  * universal and existential quantifiers
  * disjunctions
  * conditional effects
  * implications
  * equality (parser only)
* new output option `--log-level` to control which status messages to show
* new basic option `--warnings-as-errors` to abort program execution upon warnings (replaces `--warning-level`)
* new compatibility mode for parser to support more legacy domains
* new parser option `--parsing-mode` to specify whether to apply strict or compatibility parsing rules

### Internal

* PDDL parser reimplemented from scratch in stand-alone `pddl` library
* removes dependency to Boost

## 3.0.3 (2016-09-02)

### Bug Fixes

* fixes incorrect output format of conditional effects with SAS
* fixes parsing issue with unexpected whitespaces in SAS files

## 3.0.2 (2016-08-18)

### Features

* unified translation format for SAS and PDDL files
* documentation of `plasp`’s output format
* improved output syntax highlighting
* uses ASP string literals to avoid escaping PDDL identifiers

### Bug Fixes

* fixes minor formatting issues in SAS translation
* fixes issue with unsupported expression negations

## 3.0.1 (2016-06-14)

### Features

* basic support for parsing PDDL 3.1 domains and problems
* limited support for translating PDDL 3.1 domains and problems to ASP facts
* automatic language detection for PDDL and SAS
* supports input from `std::cin`
* supports colorized output
* new command-line option `--language` to explicitly specify input language
* new command-line option `--warning-level` to treat warnings as errors or to ignore warnings
* new command-line option `--color` to autodetect, enable, or disable color output

### Bug Fixes

* fixes bug in translation of SAS axiom rules

## 3.0.0 (2016-05-24)

### Features

* parses [SAS](http://www.fast-downward.org/TranslatorOutputFormat) files created with [Fast Downward](http://www.fast-downward.org/)
* experimental translation from SAS to ASP facts
