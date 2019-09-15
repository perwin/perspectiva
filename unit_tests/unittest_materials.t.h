// Unit tests for code in materials.h

#include <stdio.h>
#include <cxxtest/TestSuite.h>
#include "color.h"
#include "materials.h"

class NewTestSuite : public CxxTest::TestSuite 
{
public:

  // Tests for SimpleMaterial class
  
//     SimpleMaterial( Color surfColor, Color reflectColor, Color refractColor,
//     				float reflect, float transp )
  void testSimpleMaterial_Creation( void )
  {
    // white, no reflection or transparency
    Color  inputSurfColor = Color(1);
    Color  inputReflectColor = Color(1);
    Color  inputRefractColor = Color(1);
    Color  inputEmissColor = Color(0.5);
    float  inputReflectivity = 0.5;
    float  inputTransparency = 0.5;
    SimpleMaterial thisMaterial = SimpleMaterial(inputSurfColor, inputReflectColor, 
    							inputRefractColor, inputEmissColor, inputReflectivity, 
    							inputTransparency);

    Color surfColor = thisMaterial.GetSurfaceColor();
    TS_ASSERT_DELTA( surfColor[0], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( surfColor[1], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( surfColor[2], 1.0, 1.0e-6 );
    Color reflecColor = thisMaterial.GetReflectionColor();
    TS_ASSERT_DELTA( reflecColor[0], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( reflecColor[1], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( reflecColor[2], 1.0, 1.0e-6 );
    Color refracColor = thisMaterial.GetRefractionColor();
    TS_ASSERT_DELTA( refracColor[0], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( refracColor[1], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( refracColor[2], 1.0, 1.0e-6 );
    Color emissColor = thisMaterial.GetEmissionColor();
    TS_ASSERT_DELTA( emissColor[0], 0.5, 1.0e-6 );
    TS_ASSERT_DELTA( emissColor[1], 0.5, 1.0e-6 );
    TS_ASSERT_DELTA( emissColor[2], 0.5, 1.0e-6 );
  
    TS_ASSERT_DELTA( thisMaterial.GetReflectivity(), inputReflectivity, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial.GetTransparency(), inputTransparency, 1.0e-6 );
  }
};
