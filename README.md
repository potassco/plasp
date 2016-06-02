# plasp—Translate PDDL to ASP

[![GitHub Release](https://img.shields.io/github/release/potassco/plasp.svg?maxAge=3600)](https://github.com/potassco/plasp/releases)
[![Build Status](https://img.shields.io/travis/potassco/plasp/develop.svg?maxAge=3600&label=build (master))](https://travis-ci.org/potassco/plasp?branch=master)
[![Build Status](https://img.shields.io/travis/potassco/plasp/develop.svg?maxAge=3600&label=build (develop))](https://travis-ci.org/potassco/plasp?branch=develop)

`plasp` 3 is in early development and not intended for productive use yet.

As of now, `plasp` 3 experimentally supports the full [SAS Format](http://www.fast-downward.org/TranslatorOutputFormat) (as of version 3) used by [Fast Downward](http://www.fast-downward.org/).

Please get in touch with [Patrick Lühne](https://www.luehne.de) if you have any suggestions.

## Usage

To translate an SAS file into ASP facts, call:

```bash
$ plasp file.sas
```

For example, a PDDL instance can be solved as follows.
First, use [Fast Downward](http://www.fast-downward.org/) to translate the files from PDDL to SAS:

```bash
$ ./fast-downward.py --translate --build=release64 domain.pddl instance.pddl
```

This creates the file `output.sas`.
The translated SAS instance can now be solved incrementally with `clingo` and the meta encoding `meta-sequential-incremental.lp`:

```bash
$ plasp output.sas > instance.lp
$ clingo encodings/meta-sequential-incremental.lp instance.lp
```

## Building

`plasp` requires a C++14 compiler (preferrably GCC ≥ 6.1), the `boost` libraries (≥ 1.55), and CMake for building.

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
