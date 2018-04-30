// Unit tests for code in image_io.cpp

#include <cxxtest/TestSuite.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
using namespace std;

#include "utilities_pub.h"
#include "definitions.h"
#include "image_io.h"
#include "color.h"

const string  TEST_TINY_PNG_IMAGE("tests/tiny_image.png");
const string  TEST_TINY_PNG_REFERENCE_IMAGE("reference/tiny_image.png");


class NewTestSuite : public CxxTest::TestSuite 
{

  public:
  
  void testWriteSimplePNG( void )
  {
    Color *image = new Color[4];
    string  filename1 = TEST_TINY_PNG_IMAGE;
    int  nX = 2;
    int  nY = 2;
    
    image[0] = Color(0);
    image[1] = Color(1.0,0,0);
    image[2] = Color(0,1.0,0);
    image[3] = Color(0,0,0.5);

    SaveImagePNG(image, nX, nY, filename1);
  }

// Color * ReadImage( const std::string imageName, int &width, int &height );
  void testReadSimplePNG( void )
  {
    Color *correctImage = new Color[4];
    Color *readImage = new Color[4];
    string  filename1 = TEST_TINY_PNG_REFERENCE_IMAGE;
    int  width, height;
    int  correct_width = 2;
    int  correct_height = 2;
    
    correctImage[0] = Color(0);
    correctImage[1] = Color(100,0,0);
    correctImage[2] = Color(0,100,0);
    correctImage[3] = Color(0,0,100);

    readImage = ReadImage(filename1.c_str(), width, height);
    
    TS_ASSERT_EQUALS(width, correct_width);
    TS_ASSERT_EQUALS(width, correct_width);
    TS_ASSERT_EQUALS(readImage[0], Color(0));
    TS_ASSERT_EQUALS(readImage[1].r, 1.0);
    TS_ASSERT_EQUALS(readImage[1].g, 0.0);
    TS_ASSERT_EQUALS(readImage[1].b, 0.0);
    TS_ASSERT_EQUALS(readImage[2].r, 0.0);
    TS_ASSERT_EQUALS(readImage[2].g, 1.0);
    TS_ASSERT_EQUALS(readImage[2].b, 0.0);
    TS_ASSERT_EQUALS(readImage[3].r, 0.0);
    TS_ASSERT_EQUALS(readImage[3].g, 0.0);
    TS_ASSERT_DELTA(readImage[3].b, 0.499505, 1.0e-6);  // ignoring conversion/rounding, should be 0.5
  }

};
