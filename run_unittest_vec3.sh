#!/bin/bash

# Unit tests for the Vec3f class

# load environment-dependent definitions for CXXTESTGEN, CPP, etc.
. ./define_unittest_vars.sh

# 
echo
echo "Generating and compiling unit tests for Vec3f..."
$CXXTESTGEN --error-printer -o test_runner_config.cpp unit_tests/unittest_vec3.t.h
$CPP -o test_runner_config test_runner_config.cpp  -I. -Isrc -I/usr/local/include -I$CXXTEST
if [ $? -eq 0 ]
then
  echo "Running unit tests for Vec3f:"
  ./test_runner_config
  exit
else
  echo "Compilation of unit tests for Vec3f failed."
  exit 1
fi
