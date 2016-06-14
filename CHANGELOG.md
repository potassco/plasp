# Change Log

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
