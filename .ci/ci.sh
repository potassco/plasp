#!/bin/bash

if [ "$1" = "gcc" ]
then
	cxx=g++
	cc=gcc
elif [ "$1" = "clang" ]
then
	cxx=clang++
	cc=clang
fi

git submodule update --init --recursive
mkdir -p build/debug
cd build/debug
cmake ../.. -GNinja -DCMAKE_BUILD_TYPE=Debug -DTOKENIZE_BUILD_TESTS=ON -DPDDL_BUILD_TESTS=ON -DPLASP_BUILD_TESTS=ON -DCMAKE_CXX_COMPILER=${cxx}
ninja plasp-app && ninja run-tokenize-tests && ninja run-pddl-tests && ninja run-tests
