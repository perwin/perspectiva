#!/bin/bash

# load environment-dependent definitions for CXXTESTGEN, CPP, etc.
. ./define_unittest_vars.sh

echo
echo "Generating and compiling unit tests for Sampler class..."
$CXXTESTGEN --error-printer -o test_runner_config.cpp unit_tests/unittest_sampler.t.h
$CPP -o test_runner_config test_runner_config.cpp sampler.cpp -I. -I/usr/local/include -I$CXXTEST
if [ $? -eq 0 ]
then
  echo "Running unit tests for sampler.cpp:"
  ./test_runner_config
  exit
else
  echo "Compilation of unit tests for sampler.cpp failed."
  exit 1
fi
