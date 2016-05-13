#!/bin/bash

# load environment-dependent definitions for CXXTESTGEN, CPP, etc.
. ./define_unittest_vars.sh

echo
echo "Generating and compiling unit tests for scene-file parser..."
$CXXTESTGEN --error-printer -o test_runner_config.cpp unit_tests/unittest_scenefile_parser.t.h
$CPP -o test_runner_config test_runner_config.cpp scenefile_parser.cpp utilities.cpp -I. -I/usr/local/include -I$CXXTEST -lyaml-cpp
if [ $? -eq 0 ]
then
  echo "Running unit tests for config_file_parser:"
  ./test_runner_config
  exit
else
  echo "Compilation of unit tests for config_file_parser.cpp failed."
  exit 1
fi