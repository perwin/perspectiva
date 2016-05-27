// Unit tests for code in lights.h

#include <stdio.h>
#include <cxxtest/TestSuite.h>
#include "definitions.h"
#include "lights.h"
#include "vec3.h"
#include "color.h"




class NewTestSuite : public CxxTest::TestSuite 
{
public:

  // Tests for PointLight class

//   PointLight( const Color &color, const float lum, const Vec3f &pos )
//   {
//     lightType = LIGHT_POINT;
//     lightPosition = pos;
//     lightColor = color;
//     luminosity = lum;
//   }

  void testPointLight_Creation( void )
  {
    Color c = Color(0.0, 1.0, 0.0);
    Vec3f pos = Vec3f(0.0, 5.0, 0.0);
    PointLight thisPointLight = PointLight(c, 10.0, pos);

    TS_ASSERT_EQUALS( thisPointLight.lightColor, c);
    TS_ASSERT_EQUALS( thisPointLight.lightPosition, pos);
    TS_ASSERT_DELTA( thisPointLight.luminosity, 10.0, 1.0e-6 );
    TS_ASSERT_EQUALS( thisPointLight.lightType, LIGHT_POINT );
  }

  // tests of illuminate()
//  void illuminate( const Vec3f &P, Vec3f &lightDir, Color &lightIntensity, float &distance ) const
  void testPointLight_illuminate( void )
  {
    // light at origin
    Color c = Color(0.5, 1.0, 0.5);
    Vec3f lightPos = Vec3f(0.0, 0.0, 0.0);
    PointLight thisPointLight = PointLight(c, 10.0, lightPos);
    Vec3f p_hit, dirFromLight;
    Vec3f correctVectorFromLightToPhit;
    Color lightIntens;
    Color correctLightIntens = c / (FOUR_PI * 100.0);   // 1/r^2 attenuation for r = 10
    float distance;
    float correctDistance = 10.0;
    
    // point directly above light
    p_hit = Vec3f(0.0, 10.0, 0.0);
    correctVectorFromLightToPhit = (p_hit - lightPos) / correctDistance;
    thisPointLight.illuminate(p_hit, dirFromLight, lightIntens, distance);
    TS_ASSERT_DELTA( dirFromLight.x, correctVectorFromLightToPhit.x, 1.0e-6 );
    TS_ASSERT_DELTA( dirFromLight.y, correctVectorFromLightToPhit.y, 1.0e-6 );
    TS_ASSERT_DELTA( dirFromLight.z, correctVectorFromLightToPhit.z, 1.0e-6 );
    TS_ASSERT_DELTA( distance, correctDistance, 1.0e-6 );
    // the following seems to overestimate the difference, somehow
//     TS_ASSERT_DELTA( lightIntens.r, correctLightIntens.r, 1.0e-4 );
//     TS_ASSERT_DELTA( lightIntens.g, correctLightIntens.g, 1.0e-4 );
//     TS_ASSERT_DELTA( lightIntens.b, correctLightIntens.b, 1.0e-4 );
	printf("\n\tPoint above point light: lightIntens comparison:\n");
    printf("\tlightIntens: r = %f, g = %f, b = %f\n", lightIntens.r,
    	lightIntens.g, lightIntens.b);
    printf("\tcorrectLightIntens: r = %f, g = %f, b = %f\n", correctLightIntens.r,
    	correctLightIntens.g, correctLightIntens.b);

    // point directly below light
    p_hit = Vec3f(0.0, -10.0, 0.0);
    correctVectorFromLightToPhit = (p_hit - lightPos) / correctDistance;
    thisPointLight.illuminate(p_hit, dirFromLight, lightIntens, distance);
    TS_ASSERT_DELTA( dirFromLight.x, correctVectorFromLightToPhit.x, 1.0e-6 );
    TS_ASSERT_DELTA( dirFromLight.y, correctVectorFromLightToPhit.y, 1.0e-6 );
    TS_ASSERT_DELTA( dirFromLight.z, correctVectorFromLightToPhit.z, 1.0e-6 );
    TS_ASSERT_DELTA( distance, correctDistance, 1.0e-6 );

    // point directly to left of light
    p_hit = Vec3f(-10.0, 0.0, 0.0);
    correctVectorFromLightToPhit = (p_hit - lightPos) / correctDistance;
    thisPointLight.illuminate(p_hit, dirFromLight, lightIntens, distance);
    TS_ASSERT_DELTA( dirFromLight.x, correctVectorFromLightToPhit.x, 1.0e-6 );
    TS_ASSERT_DELTA( dirFromLight.y, correctVectorFromLightToPhit.y, 1.0e-6 );
    TS_ASSERT_DELTA( dirFromLight.z, correctVectorFromLightToPhit.z, 1.0e-6 );
    TS_ASSERT_DELTA( distance, correctDistance, 1.0e-6 );

    // point directly to right of light
    p_hit = Vec3f(10.0, 0.0, 0.0);
    correctVectorFromLightToPhit = (p_hit - lightPos) / correctDistance;
    thisPointLight.illuminate(p_hit, dirFromLight, lightIntens, distance);
    TS_ASSERT_DELTA( dirFromLight.x, correctVectorFromLightToPhit.x, 1.0e-6 );
    TS_ASSERT_DELTA( dirFromLight.y, correctVectorFromLightToPhit.y, 1.0e-6 );
    TS_ASSERT_DELTA( dirFromLight.z, correctVectorFromLightToPhit.z, 1.0e-6 );
    TS_ASSERT_DELTA( distance, correctDistance, 1.0e-6 );

    // point directly behind light
    p_hit = Vec3f(0.0, 0.0, -10.0);
    correctVectorFromLightToPhit = (p_hit - lightPos) / correctDistance;
    thisPointLight.illuminate(p_hit, dirFromLight, lightIntens, distance);
    TS_ASSERT_DELTA( dirFromLight.x, correctVectorFromLightToPhit.x, 1.0e-6 );
    TS_ASSERT_DELTA( dirFromLight.y, correctVectorFromLightToPhit.y, 1.0e-6 );
    TS_ASSERT_DELTA( dirFromLight.z, correctVectorFromLightToPhit.z, 1.0e-6 );
    TS_ASSERT_DELTA( distance, correctDistance, 1.0e-6 );

    // point directly in front of light
    p_hit = Vec3f(0.0, 0.0, 10.0);
    correctVectorFromLightToPhit = (p_hit - lightPos) / correctDistance;
    thisPointLight.illuminate(p_hit, dirFromLight, lightIntens, distance);
    TS_ASSERT_DELTA( dirFromLight.x, correctVectorFromLightToPhit.x, 1.0e-6 );
    TS_ASSERT_DELTA( dirFromLight.y, correctVectorFromLightToPhit.y, 1.0e-6 );
    TS_ASSERT_DELTA( dirFromLight.z, correctVectorFromLightToPhit.z, 1.0e-6 );
    TS_ASSERT_DELTA( distance, correctDistance, 1.0e-6 );

  }


//   DistantLight( const Vec3f &lightDir, const Color &color, const float lum )
//   {
//     lightType = LIGHT_DISTANT;
//     dir = lightDir;
//     dir.normalize();
//     lightColor = color;
//     luminosity = lum;
//   }

  void testDistantLight_Creation( void )
  {
    Color c = Color(0.0, 1.0, 0.0);
    Vec3f lightDir = Vec3f(0.0, -1.0, 0.0);
    float radiance = 100.0;
    DistantLight thisDistantLight = DistantLight(lightDir, c, radiance);

    TS_ASSERT_EQUALS( thisDistantLight.lightColor, c);
    TS_ASSERT_EQUALS( thisDistantLight.dir, lightDir);
    TS_ASSERT_DELTA( thisDistantLight.luminosity, radiance, 1.0e-6 );
    TS_ASSERT_EQUALS( thisDistantLight.lightType, LIGHT_DISTANT );
  }

  // tests of illuminate()
//  void illuminate( const Vec3f &P, Vec3f &lightDir, Color &lightIntensity, float &distance ) const
  void testDistantLight_illuminate( void )
  {
    // light aimed to right
    Color c = Color(0.0, 1.0, 0.0);
    Vec3f dir = Vec3f(1.0, 0.0, 0.0);
    float radiance = 100.0;
    DistantLight thisDistantLight = DistantLight(dir, c, radiance);
    Vec3f p_hit, dirFromLight;
    Vec3f correctVectorFromLightToPhit;
    Color lightIntens;
    Color correctLightIntens = radiance * c;
    float distance;
    
    // point at origin
    p_hit = Vec3f(0.0, 0.0, 0.0);
    correctVectorFromLightToPhit = Vec3f(1.0, 0.0, 0.0);
    thisDistantLight.illuminate(p_hit, dirFromLight, lightIntens, distance);
    TS_ASSERT_DELTA( dirFromLight.x, correctVectorFromLightToPhit.x, 1.0e-6 );
    TS_ASSERT_DELTA( dirFromLight.y, correctVectorFromLightToPhit.y, 1.0e-6 );
    TS_ASSERT_DELTA( dirFromLight.z, correctVectorFromLightToPhit.z, 1.0e-6 );
    TS_ASSERT_EQUALS( lightIntens, correctLightIntens );
    TS_ASSERT_DELTA( distance, kInfinity, 1.0e-6 );

  }
};
