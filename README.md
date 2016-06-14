# plasp—Translate PDDL to ASP

[![GitHub Release](https://img.shields.io/github/release/potassco/plasp.svg?maxAge=3600)](https://github.com/potassco/plasp/releases)
[![Build Status](https://img.shields.io/travis/potassco/plasp/develop.svg?maxAge=3600&label=build (master))](https://travis-ci.org/potassco/plasp?branch=master)
[![Build Status](https://img.shields.io/travis/potassco/plasp/develop.svg?maxAge=3600&label=build (develop))](https://travis-ci.org/potassco/plasp?branch=develop)

`plasp` 3 is in early development and not intended for productive use yet.

`plasp` 3 translates planning problem instances to ASP facts.
`plasp` 3 supports the input languages [PDDL](https://helios.hud.ac.uk/scommv/IPC-14/software.html) (only basic features currently) and the [SAS](http://www.fast-downward.org/TranslatorOutputFormat) (full support of the current version 3), which is used by [Fast Downward](http://www.fast-downward.org/).

Please get in touch with [Patrick Lühne](https://www.luehne.de) if you have any suggestions.

## Usage

### Translating PDDL to ASP Facts

PDDL instances are translated to ASP facts as follows:

```bash
$ plasp domain.pddl problem.pddl
```

Alternatively, PDDL instances may first be translated to SAS, the output format of [Fast Downward](http://www.fast-downward.org/).

```bash
$ ./fast-downward.py --translate --build=release64 domain.pddl instance.pddl
```

This creates a file called `output.sas`, which may now be translated by `plasp`.

```bash
$ plasp output.sas
```

### Solving the Translated Instance

The translated instance can finally be solved incrementally with `clingo` and a meta encoding, for instance, `pddl-meta-sequential-incremental.lp`:

```bash
$ plasp domain.pddl problem.pddl > instance.lp
$ clingo encodings/pddl-meta-sequential-incremental.lp instance.lp
```

### Command-Line Interface

```bash
$ plasp [files] [options]
```

`[files]` may be omitted, in which case the input is read from `std::cin`.
The `[options]` are listed below:

| **option**                        | **explanation**                                                                                                                   |
|-----------------------------------|-----------------------------------------------------------------------------------------------------------------------------------|
| `-l` [ `--language` ]             | Specify the input language (`sas` or `pddl`). Omit for automatic detection.                                                       |
| `--warning-level` arg (=`normal`) | Specify whether to output warnings normally (`normal`), to treat them as critical errors (`error`), or to ignore them (`ignore`). |
| `--color` arg (=`auto`)           | Specify whether to colorize the output (`always`, `never`, or `auto`).                                                            |

## Building

`plasp` requires a C++14 compiler (preferrably GCC ≥ 6.1 or clang ≥ 3.8), the `boost` libraries (≥ 1.55), and CMake for building.

```bash
$ git clone https://github.com/potassco/plasp.git
$ cd plasp
$ mkdir -p build/release
$ cd build/release
$ cmake ../.. -DCMAKE_BUILD_TYPE=Release
$ make
```

The built `plasp` binary is then located at `plasp/build/release/bin/plasp`.

### Running the Tests

`plasp` provides unit tests written using the [Google Test](https://github.com/google/googletest) framework.
Before building and running the tests, make sure you have fetched the Google Test git submodule:

```bash
$ git submodule init
$ git submodule update
```

Afterward, build and run the tests as follows:

```bash
$ make run-tests
```

## Contributors

* [Patrick Lühne](https://www.luehne.de) (`plasp` 3)

### Earlier Versions

* René Knaebel
* Murat Knecht
