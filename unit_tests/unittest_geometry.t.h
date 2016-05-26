// Unit tests for code in geometry.h

#include <stdio.h>
#include <cxxtest/TestSuite.h>
#include "geometry.h"
#include "vec3.h"
#include "color.h"



//   Sphere( const Vec3f &cen, const float &r, const Color &surfColor, const float &refl = 0, 
//     	const float &transp = 0, const Color &emissColor = 0 )


class NewTestSuite : public CxxTest::TestSuite 
{
public:

  // Tests for Sphere class
  
  void testSphere_Creation( void )
  {
    Sphere thisSphere = Sphere(Vec3f(0), 1.0, Color(1), 0, 0);

    TS_ASSERT_DELTA( thisSphere.center[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.center[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.center[2], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.radius, 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.radius2, 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.surfaceColor[0], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.surfaceColor[1], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.surfaceColor[2], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.reflection, 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.transparency, 0.0, 1.0e-6 );

    Sphere emissSphere = Sphere(Vec3f(0), 1.0, Color(1), 0, 0, Color(0.0, 1.0, 2.0));

    TS_ASSERT_DELTA( emissSphere.emissionColor[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( emissSphere.emissionColor[1], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( emissSphere.emissionColor[2], 2.0, 1.0e-6 );
  }

  void testSphere_GetNormalAtPoint( void )
  {
    Vec3f retval;
    Sphere centeredSphere = Sphere(Vec3f(0.0, 0.0, 0.0), 1.0, Color(1), 0, 0);
    Sphere offsetSphere = Sphere(Vec3f(1.0, 0.0, 0.0), 1.0, Color(1), 0, 0);

    Vec3f centered_point_top = Vec3f(0.0, 1.0, 0.0);
    Vec3f centered_point_bottom = Vec3f(0.0, -1.0, 0.0);
    Vec3f centered_point_left = Vec3f(-1.0, 0.0, 0.0);
    Vec3f centered_point_right = Vec3f(1.0, 0.0, 0.0);
    Vec3f centered_point_back = Vec3f(0.0, 0.0, -1.0);
    Vec3f offset_point_top = Vec3f(1.0, 1.0, 0.0);
    Vec3f offset_point_bottom = Vec3f(1.0, -1.0, 0.0);
    Vec3f offset_point_left = Vec3f(0.0, 0.0, 0.0);
    Vec3f offset_point_right = Vec3f(2.0, 0.0, 0.0);
    Vec3f offset_point_back = Vec3f(1.0, 0.0, -1.0);
    
    Vec3f correct_normal_top = Vec3f(0.0, 1.0, 0.0);
    Vec3f correct_normal_bottom = Vec3f(0.0, -1.0, 0.0);
    Vec3f correct_normal_left = Vec3f(-1.0, 0.0, 0.0);
    Vec3f correct_normal_right = Vec3f(1.0, 0.0, 0.0);
    Vec3f correct_normal_back = Vec3f(0.0, 0.0, -1.0);
    
    retval = centeredSphere.GetNormalAtPoint(centered_point_top);
    TS_ASSERT_EQUALS( (retval == correct_normal_top), true);
    retval = offsetSphere.GetNormalAtPoint(offset_point_top);
    TS_ASSERT_EQUALS( (retval == correct_normal_top), true);

    retval = centeredSphere.GetNormalAtPoint(centered_point_bottom);
    TS_ASSERT_EQUALS( (retval == correct_normal_bottom), true);
    retval = offsetSphere.GetNormalAtPoint(offset_point_bottom);
    TS_ASSERT_EQUALS( (retval == correct_normal_bottom), true);

    retval = centeredSphere.GetNormalAtPoint(centered_point_left);
    TS_ASSERT_EQUALS( (retval == correct_normal_left), true);
    retval = offsetSphere.GetNormalAtPoint(offset_point_left);
    TS_ASSERT_EQUALS( (retval == correct_normal_left), true);

    retval = centeredSphere.GetNormalAtPoint(centered_point_right);
    TS_ASSERT_EQUALS( (retval == correct_normal_right), true);
    retval = offsetSphere.GetNormalAtPoint(offset_point_right);
    TS_ASSERT_EQUALS( (retval == correct_normal_right), true);

    retval = centeredSphere.GetNormalAtPoint(centered_point_back);
    TS_ASSERT_EQUALS( (retval == correct_normal_back), true);
    retval = offsetSphere.GetNormalAtPoint(offset_point_back);
    TS_ASSERT_EQUALS( (retval == correct_normal_back), true);
    
  }

//   bool intersect( const Vec3f &rayorig, const Vec3f &raydir, float *t0, float *t1 ) const;
  void testSphereIntersect( void )
  {
    Sphere centeredSphere = Sphere(Vec3f(0.0, 0.0, 0.0), 1.0, Color(1), 0, 0);
    Sphere behindCameraSphere = Sphere(Vec3f(0.0, 0.0, 20.0), 1.0, Color(1), 0, 0);
    bool  intersected;
    float  t0 = 0.f, t1 = 0.f;
    Vec3f  rayOrigin = Vec3f(0, 0, 10);  // x=y=0, 10 units away from origin toward camera
    Vec3f  rayDir = Vec3f(0, 0, -1);
    rayDir.normalize();
    
    intersected = centeredSphere.intersect(rayOrigin, rayDir, &t0, &t1);
    TS_ASSERT_EQUALS(intersected, true);
    TS_ASSERT_DELTA(t0, 9.0, 1.0e-6);
    TS_ASSERT_DELTA(t1, 11.0, 1.0e-6);

    t0 = t1 = 0.0;
    intersected = behindCameraSphere.intersect(rayOrigin, rayDir, &t0, &t1);
    TS_ASSERT_EQUALS(intersected, false);
    TS_ASSERT_DELTA(t0, 0.0, 1.0e-6);
    TS_ASSERT_DELTA(t1, 0.0, 1.0e-6);
  }
};
