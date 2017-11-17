# plasp [![GitHub Release](https://img.shields.io/github/release/potassco/plasp.svg?maxAge=3600)](https://github.com/potassco/plasp/releases) [![Build Status](https://img.shields.io/travis/potassco/plasp/master.svg?maxAge=3600&label=build%20%28master%29)](https://travis-ci.org/potassco/plasp?branch=master) [![Build Status](https://img.shields.io/travis/potassco/plasp/develop.svg?maxAge=3600&label=build%20%28develop%29)](https://travis-ci.org/potassco/plasp?branch=develop)

> ASP planning tools for PDDL

## Overview

`plasp` is a tool collection for planning in [*answer set programming*](https://en.wikipedia.org/wiki/Answer_set_programming).
`plasp` 3 supports the input languages [PDDL 3.1](https://helios.hud.ac.uk/scommv/IPC-14/software.html) (except for advanced features such as durative actions, numerical fluents, and preferences) and [SAS](http://www.fast-downward.org/TranslatorOutputFormat) (full support of SAS 3), which is used by [Fast Downward](http://www.fast-downward.org/).

The most notable tool provided by `plasp` is `plasp translate`, which translates PDDL descriptions to ASP facts.

## Translating PDDL to ASP Facts

PDDL instances are translated to ASP facts as follows:

```sh
plasp translate domain.pddl problem.pddl
```

Alternatively, PDDL instances may first be translated to SAS, the output format of [Fast Downward](http://www.fast-downward.org/).

```sh
./fast-downward.py --translate --build=release64 domain.pddl problem.pddl
```

This creates a file called `output.sas`, which may now be translated by `plasp` as well.

```sh
plasp translate output.sas
```

### Solving the Translated Instance

The translated instance can finally be solved with `clingo` and a meta encoding, for instance, [`sequential-horizon.lp`](encodings/sequential-horizon.lp):

```sh
plasp translate domain.pddl problem.pddl > instance.lp
clingo encodings/sequential-horizon.lp -c horizon=10 instance.lp
```

### Translator Output Format

`plasp translate` provides a uniform output format for SAS and PDDL input problems.
See [output format](doc/output-format.md) for more details.

If you want to write your own meta encoding for `plasp translate`’s output, this [simple example encoding](encodings/sequential-horizon.lp) gets you started.

## Provided Tools

```sh
plasp <command> [<option>...] [<input file>...]
```

Aside from translating PDDL to ASP facts, `plasp` provides the following commands:

| command | description |
|---|---|
| `translate` | Translate PDDL and SAS to ASP facts |
| `normalize` | Normalize PDDL to plasp’s custom PDDL format |
| `check-syntax` | Check the syntax of PDDL specifications |
| `beautify` | Cleanly format PDDL specifications |
| `help` | Display help message |
| `version` | Display version information |

`plasp help` shows a list of all commands provided by `plasp`.
To list all available options of a command, call `plasp <command> --help` or `plasp help <command>`.

`plasp` automatically detects the language (PDDL or SAS) of the input descriptions.

## Building

`plasp` is built with CMake and a C++ compiler.
See [building](doc/building.md) for more details.

## PDDL Feature Support

`plasp` supports a subset of PDDL 3.1.
See [PDDL feature support](doc/pddl-feature-support.md) for a list of supported and unsupported PDDL 3.1 features.

## Contributors

* [Patrick Lühne](https://www.luehne.de)
* Martin Gebser (encodings)
* Torsten Schaub (encodings)

### Earlier Versions

* René Knaebel
* Murat Knecht
