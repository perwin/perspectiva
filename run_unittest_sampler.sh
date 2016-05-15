#!/bin/bash

# Unit tests for the different sampler classes

# load environment-dependent definitions for CXXTESTGEN, CPP, etc.
. ./define_unittest_vars.sh

# UniformSampler
echo
echo "Generating and compiling unit tests for UniformSampler class..."
$CXXTESTGEN --error-printer -o test_runner_config.cpp unit_tests/unittest_uniform_sampler.t.h
$CPP -o test_runner_config test_runner_config.cpp sampler.cpp uniform_sampler.cpp -I. -I/usr/local/include -I$CXXTEST
if [ $? -eq 0 ]
then
  echo "Running unit tests for UniformSampler class:"
  ./test_runner_config
  exit
else
  echo "Compilation of unit tests for UniformSampler class failed."
  exit 1
fi
