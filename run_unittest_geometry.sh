#!/bin/bash

# Unit tests for the different geometry classes (Point, Vector, etc.)

# load environment-dependent definitions for CXXTESTGEN, CPP, etc.
. ./define_unittest_vars.sh

# 
echo
echo "Generating and compiling unit tests for geometry.h classes..."
$CXXTESTGEN --error-printer -o test_runner_config.cpp unit_tests/unittest_geometry.t.h
$CPP -std=c++11 -o test_runner_config test_runner_config.cpp  -I. -I./src -I/usr/local/include -I$CXXTEST
if [ $? -eq 0 ]
then
  echo "Running unit tests for geometry.h classes:"
  ./test_runner_config
  exit
else
  echo "Compilation of unit tests for geometry.h classes failed."
  exit 1
fi
