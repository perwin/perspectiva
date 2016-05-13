#!/bin/bash

STATUS=0

# Generate output image, compare with reference
echo -n "Generating default output image..."
./raytracer2 &> test_dump
echo

echo -n "*** Diff comparison with reference image... "
if (diff --brief untitled.ppm reference.ppm)
then
  echo " OK"
else
  echo "Diff output:"
  diff temptest/test_dump0 tests/makeimage_textout0
  STATUS=1
fi
