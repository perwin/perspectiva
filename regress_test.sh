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

# Generate output image, compare with reference
echo -n "Generating output image with oversampling=1..."
./raytracer2 --width=640 --height=480 --oversample=1 -o test3 &> test_dump
echo

# Generate small image with oversampling
echo -n "Generating small output image with oversampling..."
./raytracer2 --width=80 --height=60 --FOV=30 --oversample=3 -o test4 &> test_dump
echo


echo -n "*** Diff comparison of first image with reference image... "
if (diff --brief test1.ppm reference.ppm)
then
  echo " OK"
else
  STATUS=1
fi

echo -n "*** Diff comparison of second image with reference image... "
if (diff --brief test2.ppm reference.ppm)
then
  echo " OK"
else
  STATUS=1
fi

echo -n "*** Diff comparison of third image with reference image... "
if (diff --brief test3.ppm reference.ppm)
then
  echo " OK"
else
  STATUS=1
fi

echo -n "*** Diff comparison of fourth image with reference image... "
if (diff --brief test4.ppm reference_80x60_osamp3.ppm)
then
  echo " OK"
else
  STATUS=1
fi
