# plasp—Translate PDDL to ASP

[![GitHub Release](https://img.shields.io/github/release/potassco/plasp.svg?maxAge=3600)](https://github.com/potassco/plasp/releases)
[![Build Status](https://img.shields.io/travis/potassco/plasp/develop.svg?maxAge=3600&label=build (master))](https://travis-ci.org/potassco/plasp?branch=master)
[![Build Status](https://img.shields.io/travis/potassco/plasp/develop.svg?maxAge=3600&label=build (develop))](https://travis-ci.org/potassco/plasp?branch=develop)

`plasp` 3 is in early development and not intended for productive use yet.

`plasp` translates planning problem instances to ASP facts.
`plasp` 3 supports the input languages [PDDL 3.1](https://helios.hud.ac.uk/scommv/IPC-14/software.html) (only basic features currently) and [SAS](http://www.fast-downward.org/TranslatorOutputFormat) (full support of SAS 3), which is used by [Fast Downward](http://www.fast-downward.org/).

Please get in touch with [Patrick Lühne](https://www.luehne.de) if you have any suggestions.

## Usage

### Translating PDDL to ASP Facts

PDDL instances are translated to ASP facts as follows:

```bash
$ plasp domain.pddl problem.pddl
```

Alternatively, PDDL instances may first be translated to SAS, the output format of [Fast Downward](http://www.fast-downward.org/).

```bash
$ ./fast-downward.py --translate --build=release64 domain.pddl problem.pddl
```

This creates a file called `output.sas`, which may now be translated by `plasp`.

```bash
$ plasp output.sas
```

### Solving the Translated Instance

The translated instance can finally be solved incrementally with `clingo` and a meta encoding, for instance, [`sequential-incremental.lp`](encodings/sequential-incremental.lp):

```bash
$ plasp domain.pddl problem.pddl > instance.lp
$ clingo encodings/sequential-incremental.lp instance.lp
```

## Command-Line Interface

```bash
$ plasp [files] [options]
```

`plasp` automatically detects the language of the input program.

See [command-line interface](doc/command-line-interface.md) for more details.

## Output Format

`plasp` provides a uniform output format for SAS and PDDL input problems.

See [output format](doc/output-format.md) for more details.

If you want to write your own meta encoding for `plasp`’s output, this [simple example encoding](encodings/sequential-incremental.lp) gets you started.

## Building

`plasp` requires `boost` and is built via CMake and a C++ compiler.

See [building instructions](doc/building-instructions.md) for more details.

## Contributors

* [Patrick Lühne](https://www.luehne.de)
* Torsten Schaub (encodings)

### Earlier Versions

* René Knaebel
* Murat Knecht
