// Unit tests for code in materials.h

#include <stdio.h>
#include <cxxtest/TestSuite.h>
#include "vec3.h"
#include "color.h"
#include "materials.h"

class NewTestSuite : public CxxTest::TestSuite 
{
public:

  // Tests for SimpleMaterial class
  
//     SimpleMaterial( Color surfColor, Color emissColor, Color reflectColor, 
//						Color refractColor, float reflect, float transp ) : 

  void testSimpleMaterial_Creation( void )
  {
    // white, no reflection or transparency
    float  inputReflectivity = 0.5;
    float  inputTransparency = 0.5;
    SimpleMaterial thisMaterial = SimpleMaterial(Color(1), Color(1), Color(0), 
    							Color(0), inputReflectivity, inputTransparency);

    TS_ASSERT_DELTA( thisMaterial.surfaceColor[0], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial.surfaceColor[1], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial.surfaceColor[2], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial.emissionColor[0], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial.emissionColor[1], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial.emissionColor[2], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial.reflectionColor[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial.reflectionColor[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial.reflectionColor[2], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial.refractionColor[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial.refractionColor[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial.refractionColor[2], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial.reflectivity, inputReflectivity, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial.transparency, inputTransparency, 1.0e-6 );
    
    Color surfColor = thisMaterial.GetSurfaceColor();
    TS_ASSERT_DELTA( surfColor[0], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( surfColor[0], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( surfColor[0], 1.0, 1.0e-6 );
    
    TS_ASSERT_DELTA( thisMaterial.GetReflectivity(), inputReflectivity, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial.GetTransparency(), inputTransparency, 1.0e-6 );
  }
};
