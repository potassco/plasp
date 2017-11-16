# Building

`plasp` requires a C++17 compiler (preferably GCC ≥ 6.1 or clang ≥ 3.8) and CMake for building.

```sh
git clone https://github.com/potassco/plasp.git
cd plasp
mkdir -p build/release
cd build/release
cmake ../.. -DCMAKE_BUILD_TYPE=Release
make
```

The built `plasp` binary is then located at `plasp/build/release/bin/plasp`.

To update `plasp` to the most recent version, perform the following steps:

```sh
cd plasp
git checkout master
git pull
cd build/release
cmake .
make
```

## Running the Tests

`plasp` provides unit tests written with [Catch2](https://github.com/catchorg/Catch2).
Before building and running the tests, make sure you have fetched the Catch submodule:

```sh
git submodule init
git submodule update
```

Afterward, enable the flag `BUILD_TESTS` in your CMake configuration.
Finally, build and run the tests as follows:

```sh
make run-tokenize-tests
make run-pddl-tests
make run-tests
```

After updating `plasp`, make sure to execute CMake again in order for the tests to run correctly.
Else, external files required by the test suite may be reported missing.
