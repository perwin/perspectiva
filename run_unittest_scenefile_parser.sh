#!/bin/bash

# load environment-dependent definitions for CXXTESTGEN, CPP, etc.
. ./define_unittest_vars.sh

echo
echo "Generating and compiling unit tests for scenefile_parser..."
$CXXTESTGEN --error-printer -o test_runner_config.cpp unit_tests/unittest_scenefile_parser.t.h
$CPP -std=c++11 -o test_runner_config test_runner_config.cpp src/scenefile_parser.cpp \
src/utilities.cpp src/shapes.cpp src/mersenne_twister.cpp -I. -I./src -I/usr/local/include -I$CXXTEST -lyaml-cpp
if [ $? -eq 0 ]
then
  echo "Running unit tests for scenefile_parser:"
  ./test_runner_config
  exit
else
  echo "Compilation of unit tests for scenefile_parser.cpp failed."
  exit 1
fi
