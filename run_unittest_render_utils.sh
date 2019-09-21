#!/bin/bash

# Unit tests for code in render_utils.cpp

# load environment-dependent definitions for CXXTESTGEN, CPP, etc.
. ./define_unittest_vars.sh

# 
echo
echo "Generating and compiling unit tests for render_utils.cpp code..."
$CXXTESTGEN --error-printer -o test_runner_config.cpp unit_tests/unittest_render_utils.t.h
$CPP -std=c++11 -o test_runner_config test_runner_config.cpp  src/render_utils.cpp \
src/mersenne_twister.cpp -I. -I./src -I/usr/local/include -I$CXXTEST
if [ $? -eq 0 ]
then
  echo "Running unit tests for render_utils.cpp code:"
  ./test_runner_config
  exit
else
  echo "Compilation of unit tests for render_utils.cpp code failed."
  exit 1
fi
