# Change Log

## (unreleased)

Features:

* extended PDDL parser support (`imply`, `exists`, `forall`, and `when` expressions)
* improved command-line interface
* new command-line option `--log-level` to control which status messages should be shown
* new command-line option `--warnings-as-errors` to abort program execution upon warnings (replaces `--warning-level`)

## 3.0.3 (2016-09-02)

Bug Fixes:

* fixes incorrect output format of conditional effects with SAS
* fixes parsing issue with unexpected whitespaces in SAS files

## 3.0.2 (2016-08-18)

Features:

* unified translation format for SAS and PDDL files
* documentation of `plasp`’s output format
* improved output syntax highlighting
* uses ASP string literals to avoid escaping PDDL identifiers

Bug Fixes:

* fixes minor formatting issues in SAS translation
* fixes issue with unsupported expression negations

## 3.0.1 (2016-06-14)

Features:

* basic support for parsing PDDL 3.1 domains and problems
* limited support for translating PDDL 3.1 domains and problems to ASP facts
* automatic language detection for PDDL and SAS
* supports input from `std::cin`
* supports colorized output
* new command-line option `--language` to explicitly specify input language
* new command-line option `--warning-level` to treat warnings as errors or to ignore warnings
* new command-line option `--color` to autodetect, enable, or disable color output

Bug Fixes:

* fixes bug in translation of SAS axiom rules

## 3.0.0 (2016-05-24)

Features:

* parses [SAS](http://www.fast-downward.org/TranslatorOutputFormat) files created with [Fast Downward](http://www.fast-downward.org/)
* experimental translation from SAS to ASP facts
