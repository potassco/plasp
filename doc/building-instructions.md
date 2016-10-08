# Building Instructions

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

To update `plasp` to the most recent version, perform the following steps:

```bash
$ cd plasp
$ git pull
$ cd build/release
$ cmake .
$ make
```

## Running the Tests

`plasp` provides unit tests written using the [Catch](https://github.com/philsquared/Catch) framework.
Before building and running the tests, make sure you have fetched the Catch git submodule:

```bash
$ git submodule init
$ git submodule update
```

Afterward, enable the flag `BUILD_TESTS` in your CMake configuration.
Finally, build and run the tests as follows:

```bash
$ make run-tests
```

After updating `plasp`, make sure to execute CMake again in order for the tests to run correctly.
Else, external files required by the test suite may be reported missing.
