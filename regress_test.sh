#!/bin/bash

# Predefine some ANSI color escape codes
RED='\033[0;31m'
GREEN='\033[0;0;32m'
NC='\033[0m' # No Color

# create an integer variable for status (& counting number of failed tests)
declare -i STATUS=0

echo
# Generate output image, compare with reference
echo -n "Generating multi-lights output image..."
rm ./test1.exr
./perspectiva --width=640 --height=480 -o test1.exr tests/scene_mult-lights.yml &> test_dump
echo

echo -n "Generating (smaller) multi-lights output image with oversample=2..."
rm ./test1b.exr
./perspectiva --width=400 --height=300 -o test1b.exr tests/scene_mult-lights.yml --oversample=2 &> test_dump
echo


# Generate small images with point light in different positions (making sure we
# don't recreate the lightDirection error in PointLight::illuminate)
echo -n "Generating small images with point light in different positions..."
rm ./test_point-left.exr ./test_point-above.exr ./test_point-right.exr
./perspectiva --width=15 --height=10 tests/scene_sphere-point-light-left.yml -o test_point-left.exr &> test_dump
./perspectiva --width=15 --height=10 tests/scene_sphere-point-light-right.yml -o test_point-right.exr &> test_dump
./perspectiva --width=15 --height=10 tests/scene_sphere-point-light-above.yml -o test_point-above.exr &> test_dump
echo

# Generate output image, compare with reference
# echo -n "Generating output image with explicitly set FOV..."
# rm ./test2.ppm
# ./perspectiva --width=640 --height=480 --FOV=30 -o test2 &> test_dump
# echo
# 
# Generate output image, compare with reference
# echo -n "Generating output image with oversampling=1..."
# rm ./test3.ppm
# ./perspectiva --width=640 --height=480 --oversample=1 -o test3 &> test_dump
# echo
# 
# Generate small image with oversampling, compare with reference
# echo -n "Generating small output image with oversampling=3..."
# rm ./test4.ppm
# ./perspectiva --width=80 --height=60 --FOV=30 --oversample=3 -o test4 &> test_dump
# echo
# 
# Same, but with PNG output
# echo -n "Same, but with PNG output..."
# rm ./test4.png
# ./perspectiva --width=80 --height=60 --FOV=30 --oversample=3 -o test4.png &> test_dump
# echo


echo
echo -n "*** Diff comparison of first image with reference image... "
if (diff --brief test1.exr reference/reference_multilights.exr)
then
  echo " OK"
else
  STATUS+=1
fi

echo -n "*** Diff comparison of second image (smaller, oversample=2) with reference image... "
if (diff --brief test1b.exr reference/reference_multilights_1b.exr)
then
  echo " OK"
else
  STATUS+=1
fi

echo -n "*** Diff comparison of small point-light-image with point-light (left) reference image... "
if (diff --brief test_point-left.exr reference/ref_small_point-light-left.exr)
then
  echo " OK"
else
  STATUS+=1
fi

echo -n "*** Diff comparison of small point-light-image with point-light (right) reference image... "
if (diff --brief test_point-right.exr reference/ref_small_point-light-right.exr)
then
  echo " OK"
else
  STATUS+=1
fi

echo -n "*** Diff comparison of small point-light-image with point-light (above) reference image... "
if (diff --brief test_point-above.exr reference/ref_small_point-light-above.exr)
then
  echo " OK"
else
  STATUS+=1
fi

echo ""
if [ $STATUS -eq 1 ]
then
  echo -e "${RED}One test failed!${NC}"
elif [ $STATUS -gt 1 ]
then
  echo -e "${RED}${STATUS} tests failed!${NC}"
else
  echo -e "${GREEN}All tests passed.${NC}"
fi

echo ""
echo "Done."
echo ""

exit $STATUS
