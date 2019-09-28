// Unit tests for code in materials.h

#include <stdio.h>
#include <cxxtest/TestSuite.h>
#include "color.h"
#include "geometry.h"
#include "materials.h"

const float COS_45 = 0.7071067811865476;


class NewTestSuite : public CxxTest::TestSuite 
{
public:

  
//       baseColor = Color(1);
//       metallic = false;
//       roughness = 0.0;
//       translucent = false;
//       refractionColor = Color(0);
//       IOR = DEFAULT_IOR;
//       emitter = false;
//       emissionColor = Color(0);

  void testMaterial_Creation_default( void )
  {
    Material thisMaterial = Material();

    TS_ASSERT_EQUALS( thisMaterial.metallic, false );
    TS_ASSERT_EQUALS( thisMaterial.translucent, false );
    TS_ASSERT_EQUALS( thisMaterial.emitter, false );
    TS_ASSERT_EQUALS( thisMaterial.IOR, DEFAULT_DIFFUSE_IOR );

    Vector ray_dir_down = Vector(0,-1,0);  // pointing straight down
    Vector ray_dir_45 = Normalize(Vector(1,-1,0));  // pointing down & to right
    Vector n_hit = Vector(0,1,0);  // pointing straight up (+y)
    // lightDirection vectors are assumed to be defined pointing FROM the light
    // TO the surface!
    Vector light_dir_down = Normalize(Vector(0,-1,0));  // pointing straight down from light
    Vector light_dir_45 = Normalize(Vector(-1,-1,0));  // pointing down & to left from light
    
    // Light coming in parallel to normal --> cos(0) * baseColor
    Color diffColor = thisMaterial.GetDiffuseColor(ray_dir_down, n_hit, light_dir_down);
    TS_ASSERT_DELTA( diffColor[0], 1.0f, 1.0e-6 );
    TS_ASSERT_DELTA( diffColor[1], 1.0f, 1.0e-6 );
    TS_ASSERT_DELTA( diffColor[2], 1.0f, 1.0e-6 );
    // Light coming in at 45 deg to normal --> cos(45) * baseColor
    diffColor = thisMaterial.GetDiffuseColor(ray_dir_45, n_hit, light_dir_45);
    TS_ASSERT_DELTA( diffColor[0], COS_45, 1.0e-6 );
    TS_ASSERT_DELTA( diffColor[1], COS_45, 1.0e-6 );
    TS_ASSERT_DELTA( diffColor[2], COS_45, 1.0e-6 );
    
    Color reflecColor = thisMaterial.GetReflectionColor(ray_dir_45, n_hit, light_dir_45);
    TS_ASSERT_DELTA( reflecColor[0], 1.0f, 1.0e-6 );
    TS_ASSERT_DELTA( reflecColor[1], 1.0f, 1.0e-6 );
    TS_ASSERT_DELTA( reflecColor[2], 1.0f, 1.0e-6 );
    Color refracColor = thisMaterial.GetRefractionColor(ray_dir_45, n_hit, light_dir_45);
    TS_ASSERT_DELTA( refracColor[0], 0.0f, 1.0e-6 );
    TS_ASSERT_DELTA( refracColor[1], 0.0f, 1.0e-6 );
    TS_ASSERT_DELTA( refracColor[2], 0.0f, 1.0e-6 );
    Color emissColor = thisMaterial.GetEmissionColor();
    TS_ASSERT_DELTA( emissColor[0], 0.0f, 1.0e-6 );
    TS_ASSERT_DELTA( emissColor[1], 0.0f, 1.0e-6 );
    TS_ASSERT_DELTA( emissColor[2], 0.0f, 1.0e-6 );
  }

  void testMaterial_Creation_diffuse( void )
  {
    Color baseC = Color(0.5, 0.2, 0.3);
    Material thisMaterial = Material(baseC, false, 0.0);

    TS_ASSERT_EQUALS( thisMaterial.metallic, false );
    TS_ASSERT_EQUALS( thisMaterial.translucent, false );
    TS_ASSERT_EQUALS( thisMaterial.emitter, false );
    TS_ASSERT_EQUALS( thisMaterial.IOR, DEFAULT_DIFFUSE_IOR );

    Vector ray_dir_down = Vector(0,-1,0);  // pointing straight down
    Vector ray_dir_45 = Normalize(Vector(1,-1,0));  // pointing down & to right
    Vector n_hit = Vector(0,1,0);  // pointing straight up (+y)
    // lightDirection vectors are assumed to be defined pointing FROM the light
    // TO the surface!
    Vector light_dir_down = Normalize(Vector(0,-1,0));  // pointing straight down from light
    Vector light_dir_45 = Normalize(Vector(-1,-1,0));  // pointing down & to left from light
    
    // Light coming in parallel to normal --> cos(0) * baseColor
    Color diffColor = thisMaterial.GetDiffuseColor(ray_dir_down, n_hit, light_dir_down);
    TS_ASSERT_DELTA( diffColor[0], baseC[0], 1.0e-6 );
    TS_ASSERT_DELTA( diffColor[1], baseC[1], 1.0e-6 );
    TS_ASSERT_DELTA( diffColor[2], baseC[2], 1.0e-6 );
    // Light coming in at 45 deg to normal --> cos(45) * baseColor
    diffColor = thisMaterial.GetDiffuseColor(ray_dir_45, n_hit, light_dir_45);
    TS_ASSERT_DELTA( diffColor[0], COS_45*baseC[0], 1.0e-6 );
    TS_ASSERT_DELTA( diffColor[1], COS_45*baseC[1], 1.0e-6 );
    TS_ASSERT_DELTA( diffColor[2], COS_45*baseC[2], 1.0e-6 );
    
    Color reflecColor = thisMaterial.GetReflectionColor(ray_dir_45, n_hit, light_dir_45);
    TS_ASSERT_DELTA( reflecColor[0], baseC[0], 1.0e-6 );
    TS_ASSERT_DELTA( reflecColor[1], baseC[1], 1.0e-6 );
    TS_ASSERT_DELTA( reflecColor[2], baseC[2], 1.0e-6 );
    Color refracColor = thisMaterial.GetRefractionColor(ray_dir_45, n_hit, light_dir_45);
    TS_ASSERT_DELTA( refracColor[0], 0.0f, 1.0e-6 );
    TS_ASSERT_DELTA( refracColor[1], 0.0f, 1.0e-6 );
    TS_ASSERT_DELTA( refracColor[2], 0.0f, 1.0e-6 );
    Color emissColor = thisMaterial.GetEmissionColor();
    TS_ASSERT_DELTA( emissColor[0], 0.0f, 1.0e-6 );
    TS_ASSERT_DELTA( emissColor[1], 0.0f, 1.0e-6 );
    TS_ASSERT_DELTA( emissColor[2], 0.0f, 1.0e-6 );
  }
};
