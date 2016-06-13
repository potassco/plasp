# Change Log

## (unreleased)

Features:

* supports input from std::cin
* basic support for parsing PDDL 3.1 domains and problems
* limited support for translating PDDL 3.1 domains and problems to ASP facts
* automatic language detection for PDDL and SAS
* new command-line option `--language` to explicitly specify input language
* new command-line option `--warning-level` to treat warnings as errors or to ignore warnings

## 3.0.0 (2016-05-24)

Features:

* parses [SAS](http://www.fast-downward.org/TranslatorOutputFormat) files created with [Fast Downward](http://www.fast-downward.org/)
* experimental translation from SAS to ASP facts
