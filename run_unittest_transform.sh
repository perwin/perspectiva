#!/bin/bash

# Unit tests for the different transform.h classes (Matrix4x4, Transform)

# load environment-dependent definitions for CXXTESTGEN, CPP, etc.
. ./define_unittest_vars.sh

# 
echo
echo "Generating and compiling unit tests for transform.h classes..."
$CXXTESTGEN --error-printer -o test_runner_config.cpp unit_tests/unittest_transform.t.h
$CPP -std=c++11 -o test_runner_config test_runner_config.cpp  src/transform.cpp -I. -I./src -I/usr/local/include -I$CXXTEST
if [ $? -eq 0 ]
then
  echo "Running unit tests for transform.h classes:"
  ./test_runner_config
  exit
else
  echo "Compilation of unit tests for transform.h classes failed."
  exit 1
fi
