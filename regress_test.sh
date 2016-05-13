#!/bin/bash

STATUS=0

echo
# Generate output image, compare with reference
echo -n "Generating default output image..."
./raytracer2 --width=640 --height=480 -o test1 &> test_dump
echo

# Generate output image, compare with reference
echo -n "Generating output image with explicitly set FOV..."
./raytracer2 --width=640 --height=480 --FOV=30 -o test2 &> test_dump
echo


echo -n "*** Diff comparison of first image with reference image... "
if (diff --brief test1.ppm reference.ppm)
then
  echo " OK"
else
  echo "Diff output:"
  diff untitled.ppm reference.ppm
  STATUS=1
fi

echo -n "*** Diff comparison of second image with reference image... "
if (diff --brief test2.ppm reference.ppm)
then
  echo " OK"
else
  echo "Diff output:"
  diff untitled.ppm reference.ppm
  STATUS=1
fi
