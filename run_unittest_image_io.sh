#!/bin/bash

# Unit tests functions in image_io.h/cpp

# load environment-dependent definitions for CXXTESTGEN, CPP, etc.
. ./define_unittest_vars.sh

# 
echo
echo "Generating and compiling unit tests for image_io.h/cpp functions..."
$CXXTESTGEN --error-printer -o test_runner_config.cpp unit_tests/unittest_image_io.t.h
$CPP -std=c++11 -o test_runner_config test_runner_config.cpp src/image_io.cpp  \
-I. -I./src -I/usr/local/include -I$CXXTEST -lIlmImf
if [ $? -eq 0 ]
then
  echo "Running unit tests for image_io.h/cpp functions:"
  ./test_runner_config
  exit
else
  echo "Compilation of unit tests for image_io.h/cpp functions failed."
  exit 1
fi
