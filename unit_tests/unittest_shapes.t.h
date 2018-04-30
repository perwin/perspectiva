// Unit tests for code in shapes.h and shapes.cpp

#include <stdio.h>
#include <cxxtest/TestSuite.h>
#include "shapes.h"
#include "geometry.h"
#include "color.h"



//   Sphere( const Point &cen, const float &r, const Color &surfColor, const float &refl = 0, 
//     	const float &transp = 0, const Color &emissColor = 0 )


class NewTestSuite : public CxxTest::TestSuite 
{
public:

  // Tests for Sphere class
  
  void testSphere_Creation( void )
  {
    Sphere thisSphere = Sphere(Point(0), 1.0, Color(1), 0, 0);

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

    Sphere emissSphere = Sphere(Point(0), 1.0, Color(1), 0, 0, Color(0.0, 1.0, 2.0));

    TS_ASSERT_DELTA( emissSphere.emissionColor[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( emissSphere.emissionColor[1], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( emissSphere.emissionColor[2], 2.0, 1.0e-6 );
  }

  void testSphere_GetNormalAtPoint( void )
  {
    Vector retval;
    Sphere centeredSphere = Sphere(Point(0.0, 0.0, 0.0), 1.0, Color(1), 0, 0);
    Sphere offsetSphere = Sphere(Point(1.0, 0.0, 0.0), 1.0, Color(1), 0, 0);

    Point centered_point_top = Point(0.0, 1.0, 0.0);
    Point centered_point_bottom = Point(0.0, -1.0, 0.0);
    Point centered_point_left = Point(-1.0, 0.0, 0.0);
    Point centered_point_right = Point(1.0, 0.0, 0.0);
    Point centered_point_back = Point(0.0, 0.0, -1.0);
    Point offset_point_top = Point(1.0, 1.0, 0.0);
    Point offset_point_bottom = Point(1.0, -1.0, 0.0);
    Point offset_point_left = Point(0.0, 0.0, 0.0);
    Point offset_point_right = Point(2.0, 0.0, 0.0);
    Point offset_point_back = Point(1.0, 0.0, -1.0);
    
    Vector correct_normal_top = Vector(0.0, 1.0, 0.0);
    Vector correct_normal_bottom = Vector(0.0, -1.0, 0.0);
    Vector correct_normal_left = Vector(-1.0, 0.0, 0.0);
    Vector correct_normal_right = Vector(1.0, 0.0, 0.0);
    Vector correct_normal_back = Vector(0.0, 0.0, -1.0);
    
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

//   bool intersect( const Point &rayorig, const Vector &raydir, float *t0, float *t1 ) const;
  void testSphereIntersect( void )
  {
    // camera (= rayorigin) at (0,0,0)
    Sphere forwardSphere = Sphere(Point(0.0, 0.0, -10.0), 1.0, Color(1), 0, 0);
    Sphere behindCameraSphere = Sphere(Point(0.0, 0.0, 20.0), 1.0, Color(1), 0, 0);
    Sphere aboveCameraSphere = Sphere(Point(0.0, 10.0, 0.0), 1.0, Color(1), 0, 0);
    Sphere belowCameraSphere = Sphere(Point(0.0, -10.0, 0.0), 1.0, Color(1), 0, 0);
    bool  intersected;
    float  t0, t1;
    Point  rayOrigin = Point(0, 0, 0);
    Vector  rayDir_forward = Normalize(Vector(0, 0, -1));
//    rayDir_forward.normalize();
    Vector  rayDir_up = Normalize(Vector(0, 1, 0));
//    rayDir_up.normalize();
    Vector  rayDir_down = Normalize(Vector(0, -1, 0));
//    rayDir_down.normalize();
    
    // sphere at x=y=0, z = -10 (10 units in front of camera along -z axis)
    t0 = t1 = 0.0;
    intersected = forwardSphere.intersect(rayOrigin, rayDir_forward, &t0, &t1);
    TS_ASSERT_EQUALS(intersected, true);
    TS_ASSERT_DELTA(t0, 9.0, 1.0e-6);
    TS_ASSERT_DELTA(t1, 11.0, 1.0e-6);
    t0 = t1 = 0.0;
    intersected = forwardSphere.intersect(rayOrigin, rayDir_up, &t0, &t1);
    TS_ASSERT_EQUALS(intersected, false);
    TS_ASSERT_DELTA(t0, 0.0, 1.0e-6);
    TS_ASSERT_DELTA(t1, 0.0, 1.0e-6);
    t0 = t1 = 0.0;
    intersected = forwardSphere.intersect(rayOrigin, rayDir_down, &t0, &t1);
    TS_ASSERT_EQUALS(intersected, false);
    TS_ASSERT_DELTA(t0, 0.0, 1.0e-6);
    TS_ASSERT_DELTA(t1, 0.0, 1.0e-6);

    // sphere at x=y=0, z = 20, located *behind* camera along +z axis
    t0 = t1 = 0.0;
    intersected = behindCameraSphere.intersect(rayOrigin, rayDir_forward, &t0, &t1);
    TS_ASSERT_EQUALS(intersected, false);
    TS_ASSERT_DELTA(t0, 0.0, 1.0e-6);
    TS_ASSERT_DELTA(t1, 0.0, 1.0e-6);
    t0 = t1 = 0.0;
    intersected = behindCameraSphere.intersect(rayOrigin, rayDir_up, &t0, &t1);
    TS_ASSERT_EQUALS(intersected, false);
    TS_ASSERT_DELTA(t0, 0.0, 1.0e-6);
    TS_ASSERT_DELTA(t1, 0.0, 1.0e-6);
    t0 = t1 = 0.0;
    intersected = behindCameraSphere.intersect(rayOrigin, rayDir_down, &t0, &t1);
    TS_ASSERT_EQUALS(intersected, false);
    TS_ASSERT_DELTA(t0, 0.0, 1.0e-6);
    TS_ASSERT_DELTA(t1, 0.0, 1.0e-6);

	// sphere at x=z=0, located at y=10 (directly above camera)
    t0 = t1 = 0.0;
    intersected = aboveCameraSphere.intersect(rayOrigin, rayDir_forward, &t0, &t1);
    TS_ASSERT_EQUALS(intersected, false);
    TS_ASSERT_DELTA(t0, 0.0, 1.0e-6);
    TS_ASSERT_DELTA(t1, 0.0, 1.0e-6);
    t0 = t1 = 0.0;
    intersected = aboveCameraSphere.intersect(rayOrigin, rayDir_up, &t0, &t1);
    TS_ASSERT_EQUALS(intersected, true);
    TS_ASSERT_DELTA(t0, 9.0, 1.0e-6);
    TS_ASSERT_DELTA(t1, 11.0, 1.0e-6);
    t0 = t1 = 0.0;
    intersected = aboveCameraSphere.intersect(rayOrigin, rayDir_down, &t0, &t1);
    TS_ASSERT_EQUALS(intersected, false);
    TS_ASSERT_DELTA(t0, 0.0, 1.0e-6);
    TS_ASSERT_DELTA(t1, 0.0, 1.0e-6);

	// sphere at x=z=0, located at y = -10 (directly below camera)
    t0 = t1 = 0.0;
    intersected = belowCameraSphere.intersect(rayOrigin, rayDir_forward, &t0, &t1);
    TS_ASSERT_EQUALS(intersected, false);
    TS_ASSERT_DELTA(t0, 0.0, 1.0e-6);
    TS_ASSERT_DELTA(t1, 0.0, 1.0e-6);
    t0 = t1 = 0.0;
    intersected = belowCameraSphere.intersect(rayOrigin, rayDir_up, &t0, &t1);
    TS_ASSERT_EQUALS(intersected, false);
    TS_ASSERT_DELTA(t0, 0.0, 1.0e-6);
    TS_ASSERT_DELTA(t1, 0.0, 1.0e-6);
    t0 = t1 = 0.0;
    intersected = belowCameraSphere.intersect(rayOrigin, rayDir_down, &t0, &t1);
    TS_ASSERT_EQUALS(intersected, true);
    TS_ASSERT_DELTA(t0, 9.0, 1.0e-6);
    TS_ASSERT_DELTA(t1, 11.0, 1.0e-6);
  }
};
