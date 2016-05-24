# plasp—Translate PDDL to ASP

[![Build Status](https://travis-ci.org/potassco/plasp.svg?branch=master)](https://travis-ci.org/potassco/plasp)

`plasp` 3 is in early development and not intended for productive use yet.

As of now, `plasp` 3 experimentally supports the full [SAS Format](http://www.fast-downward.org/TranslatorOutputFormat) (as of version 3) used by [Fast Downward](http://www.fast-downward.org/).

Please get in touch with [Patrick Lühne](https://www.luehne.de) if you have any suggestions.

## Usage

To translate an SAS file into ASP facts, call:

```bash
$ plasp file.sas
```

For instance, a PDDL instance can be solved as follows.
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

## Contributors

* [Patrick Lühne](https://www.luehne.de) (`plasp` 3)

### Earlier Versions

* René Knaebel (`plasp` 2)
* Murat Knecht (`plasp`)
