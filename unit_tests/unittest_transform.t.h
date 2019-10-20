// Unit tests for code in transform.h

#include <stdio.h>
#include <cxxtest/TestSuite.h>

#include "geometry.h"
#include "transform.h"
#include "definitions.h"


class TestMatrix4x4Class : public CxxTest::TestSuite 
{
public:

  void TestMatrixCreation( void )
  {
    Matrix4x4 m;
    
    TS_ASSERT_EQUALS(m.m[0][0], 1.0);
    TS_ASSERT_EQUALS(m.m[1][1], 1.0);
    TS_ASSERT_EQUALS(m.m[2][2], 1.0);
    TS_ASSERT_EQUALS(m.m[3][3], 1.0);
    TS_ASSERT_EQUALS(m.m[0][1], 0.0);
    TS_ASSERT_EQUALS(m.m[3][0], 0.0);
  }
};





class TestTransformClass : public CxxTest::TestSuite 
{
public:

  void TestTransformCreation_identity_matrix( void )
  {
    Transform t;
    
    TS_ASSERT_EQUALS(t.matrix.m[0][0], 1.0);
    TS_ASSERT_EQUALS(t.matrix.m[1][1], 1.0);
    TS_ASSERT_EQUALS(t.matrix.m[2][2], 1.0);
    TS_ASSERT_EQUALS(t.matrix.m[3][3], 1.0);
    TS_ASSERT_EQUALS(t.matrix.m[0][1], 0.0);
    TS_ASSERT_EQUALS(t.matrix.m[3][0], 0.0);
  }
  
  // test transforms using the identity matrix
  void TestTransform_transformPoint_identity( void )
  {
    Transform t;
    Point p0 = Point(0);
    Point p1 = Point(1);
    
    Point p0_t = t(p0);
    Point p1_t = t(p1);

    TS_ASSERT_EQUALS(p0_t, p0);
    TS_ASSERT_EQUALS(p1_t, p1);
  }
  
  void TestTransform_transformVector_identity( void )
  {
    Transform t;
    Vector v0 = Vector(0);
    Vector v1 = Vector(1);
    
    Vector v0_t = t(v0);
    Vector v1_t = t(v1);

    TS_ASSERT_EQUALS(v0_t, v0);
    TS_ASSERT_EQUALS(v1_t, v1);
  }

  // test translation transform
  void TestTransform_transformPoint_translation( void )
  {
    Vector delta = Vector(1.0, 10.0, -5.0);
    Transform t(delta);
    Point p0 = Point(0);
    Point p1 = Point(1);
    
    Point p0_t = t(p0);
    Point p0_t_correct = Point(0.0 + 1.0, 0.0 + 10.0, 0.0 - 5.0);
    Point p1_t = t(p1);
    Point p1_t_correct = Point(1.0 + 1.0, 1.0 + 10.0, 1.0 - 5.0);

    TS_ASSERT_EQUALS(p0_t, p0_t_correct);
    TS_ASSERT_EQUALS(p1_t, p1_t_correct);
  }

  void TestTransform_transformVector_translation( void )
  {
    Vector delta = Vector(1.0, 10.0, -5.0);
    Transform t(delta);
    Vector v0 = Vector(0);
    Vector v1 = Vector(1);
    
    // translation of vectors should leave them unchanged, so "correct" answers
    // are the original vectors
    Vector v0_t = t(v0);
    Vector v1_t = t(v1);

    TS_ASSERT_EQUALS(v0_t, v0);
    TS_ASSERT_EQUALS(v1_t, v1);
  }

};



class TestTransformFuncs : public CxxTest::TestSuite 
{
public:

  void TestTransformFuncs_rotateX_points( void )
  {
    Transform t_rot0, t_rot90, t_rot180;
    float  ang0 = 0.0, ang90 = PI_OVER_TWO, ang180 = PI;
    Point p0 = Point(0);
    Point p1 = Point(1);
    // p1 rotated by 90 deg around x-axis
    Point p1_rot90_correct = Point(1, -1, 1);
    // p1 rotated by 180 deg around x-axis
    Point p1_rot180_correct = Point(1, -1, -1);
    
    t_rot0 = RotateX(ang0);
    t_rot90 = RotateX(ang90);
    t_rot180 = RotateX(ang180);
    
    // Points should be unchanged by rotation = 0
    Point p0_t = t_rot0(p0);
    Point p1_t = t_rot0(p1);
    TS_ASSERT_EQUALS(p0_t, p0);
    TS_ASSERT_EQUALS(p1_t, p1);

    p0_t = t_rot90(p0);
    p1_t = t_rot90(p1);
    TS_ASSERT_EQUALS(p0_t, p0);
    TS_ASSERT_DELTA(p1_t.x, p1_rot90_correct.x, 1.0e-5);
    TS_ASSERT_DELTA(p1_t.y, p1_rot90_correct.y, 1.0e-5);
    TS_ASSERT_DELTA(p1_t.z, p1_rot90_correct.z, 1.0e-5);

    p0_t = t_rot180(p0);
    p1_t = t_rot180(p1);
    TS_ASSERT_EQUALS(p0_t, p0);
    TS_ASSERT_DELTA(p1_t.x, p1_rot180_correct.x, 1.0e-5);
    TS_ASSERT_DELTA(p1_t.y, p1_rot180_correct.y, 1.0e-5);
    TS_ASSERT_DELTA(p1_t.z, p1_rot180_correct.z, 1.0e-5);
  }

  void TestTransformFuncs_rotateX_vectors( void )
  {
    Transform t_rot0, t_rot90, t_rot180;
    float  ang0 = 0.0, ang90 = PI_OVER_TWO, ang180 = PI;
    Vector v0 = Vector(0);
    Vector v1 = Vector(1);
    // v1 rotated by 90 deg around x-axis
    Vector v1_rot90_correct = Vector(1, -1, 1);
    // v1 rotated by 180 deg around x-axis
    Vector v1_rot180_correct = Vector(1, -1, -1);
    
    t_rot0 = RotateX(ang0);
    t_rot90 = RotateX(ang90);
    t_rot180 = RotateX(ang180);
    
    // Vectors should be unchanged by rotation = 0
    Vector v0_t = t_rot0(v0);
    Vector v1_t = t_rot0(v1);
    TS_ASSERT_EQUALS(v0_t, v0);
    TS_ASSERT_EQUALS(v1_t, v1);

    v0_t = t_rot90(v0);
    v1_t = t_rot90(v1);
    TS_ASSERT_EQUALS(v0_t, v0);
    TS_ASSERT_DELTA(v1_t.x, v1_rot90_correct.x, 1.0e-5);
    TS_ASSERT_DELTA(v1_t.y, v1_rot90_correct.y, 1.0e-5);
    TS_ASSERT_DELTA(v1_t.z, v1_rot90_correct.z, 1.0e-5);

    v0_t = t_rot180(v0);
    v1_t = t_rot180(v1);
    TS_ASSERT_EQUALS(v0_t, v0);
    TS_ASSERT_DELTA(v1_t.x, v1_rot180_correct.x, 1.0e-5);
    TS_ASSERT_DELTA(v1_t.y, v1_rot180_correct.y, 1.0e-5);
    TS_ASSERT_DELTA(v1_t.z, v1_rot180_correct.z, 1.0e-5);
  }

  void TestTransformFuncs_rotateY_points( void )
  {
    Transform t_rot0, t_rot90, t_rot180;
    float  ang0 = 0.0, ang90 = PI_OVER_TWO, ang180 = PI;
    Point p0 = Point(0);
    Point p1 = Point(1);
    // p1 rotated by 90 deg around y-axis
    Point p1_rot90_correct = Point(1, 1, -1);
    // p1 rotated by 180 deg around y-axis
    Point p1_rot180_correct = Point(-1, 1, -1);
    
    t_rot0 = RotateY(ang0);
    t_rot90 = RotateY(ang90);
    t_rot180 = RotateY(ang180);
    
    // Points should be unchanged by rotation = 0
    Point p0_t = t_rot0(p0);
    Point p1_t = t_rot0(p1);
    TS_ASSERT_EQUALS(p0_t, p0);
    TS_ASSERT_EQUALS(p1_t, p1);

    p0_t = t_rot90(p0);
    p1_t = t_rot90(p1);
    TS_ASSERT_EQUALS(p0_t, p0);
    TS_ASSERT_DELTA(p1_t.x, p1_rot90_correct.x, 1.0e-5);
    TS_ASSERT_DELTA(p1_t.y, p1_rot90_correct.y, 1.0e-5);
    TS_ASSERT_DELTA(p1_t.z, p1_rot90_correct.z, 1.0e-5);

    p0_t = t_rot180(p0);
    p1_t = t_rot180(p1);
    TS_ASSERT_EQUALS(p0_t, p0);
    TS_ASSERT_DELTA(p1_t.x, p1_rot180_correct.x, 1.0e-5);
    TS_ASSERT_DELTA(p1_t.y, p1_rot180_correct.y, 1.0e-5);
    TS_ASSERT_DELTA(p1_t.z, p1_rot180_correct.z, 1.0e-5);
  }

  void TestTransformFuncs_rotateY_vectors( void )
  {
    Transform t_rot0, t_rot90, t_rot180;
    float  ang0 = 0.0, ang90 = PI_OVER_TWO, ang180 = PI;
    Vector v0 = Vector(0);
    Vector v1 = Vector(1);
    // v1 rotated by 90 deg around y-axis
    Vector v1_rot90_correct = Vector(1, 1, -1);
    // v1 rotated by 180 deg around y-axis
    Vector v1_rot180_correct = Vector(-1, 1, -1);
    
    t_rot0 = RotateY(ang0);
    t_rot90 = RotateY(ang90);
    t_rot180 = RotateY(ang180);
    
    // Vectors should be unchanged by rotation = 0
    Vector v0_t = t_rot0(v0);
    Vector v1_t = t_rot0(v1);
    TS_ASSERT_EQUALS(v0_t, v0);
    TS_ASSERT_EQUALS(v1_t, v1);

    v0_t = t_rot90(v0);
    v1_t = t_rot90(v1);
    TS_ASSERT_EQUALS(v0_t, v0);
    TS_ASSERT_DELTA(v1_t.x, v1_rot90_correct.x, 1.0e-5);
    TS_ASSERT_DELTA(v1_t.y, v1_rot90_correct.y, 1.0e-5);
    TS_ASSERT_DELTA(v1_t.z, v1_rot90_correct.z, 1.0e-5);

    v0_t = t_rot180(v0);
    v1_t = t_rot180(v1);
    TS_ASSERT_EQUALS(v0_t, v0);
    TS_ASSERT_DELTA(v1_t.x, v1_rot180_correct.x, 1.0e-5);
    TS_ASSERT_DELTA(v1_t.y, v1_rot180_correct.y, 1.0e-5);
    TS_ASSERT_DELTA(v1_t.z, v1_rot180_correct.z, 1.0e-5);
  }

  void TestTransformFuncs_rotateZ_points( void )
  {
    Transform t_rot0, t_rot90, t_rot180;
    float  ang0 = 0.0, ang90 = PI_OVER_TWO, ang180 = PI;
    Point p0 = Point(0);
    Point p1 = Point(1);
    // p1 rotated by 90 deg around z-axis
    Point p1_rot90_correct = Point(-1, 1, 1);
    // p1 rotated by 180 deg around z-axis
    Point p1_rot180_correct = Point(-1, -1, 1);
    
    t_rot0 = RotateZ(ang0);
    t_rot90 = RotateZ(ang90);
    t_rot180 = RotateZ(ang180);
    
    // Points should be unchanged by rotation = 0
    Point p0_t = t_rot0(p0);
    Point p1_t = t_rot0(p1);
    TS_ASSERT_EQUALS(p0_t, p0);
    TS_ASSERT_EQUALS(p1_t, p1);

    p0_t = t_rot90(p0);
    p1_t = t_rot90(p1);
    TS_ASSERT_EQUALS(p0_t, p0);
    TS_ASSERT_DELTA(p1_t.x, p1_rot90_correct.x, 1.0e-5);
    TS_ASSERT_DELTA(p1_t.y, p1_rot90_correct.y, 1.0e-5);
    TS_ASSERT_DELTA(p1_t.z, p1_rot90_correct.z, 1.0e-5);

    p0_t = t_rot180(p0);
    p1_t = t_rot180(p1);
    TS_ASSERT_EQUALS(p0_t, p0);
    TS_ASSERT_DELTA(p1_t.x, p1_rot180_correct.x, 1.0e-5);
    TS_ASSERT_DELTA(p1_t.y, p1_rot180_correct.y, 1.0e-5);
    TS_ASSERT_DELTA(p1_t.z, p1_rot180_correct.z, 1.0e-5);
  }

  void TestTransformFuncs_rotateZ_vectors( void )
  {
    Transform t_rot0, t_rot90, t_rot180;
    float  ang0 = 0.0, ang90 = PI_OVER_TWO, ang180 = PI;
    Vector v0 = Vector(0);
    Vector v1 = Vector(1);
    // v1 rotated by 90 deg around z-axis
    Vector v1_rot90_correct = Vector(-1, 1, 1);
    // v1 rotated by 180 deg around z-axis
    Vector v1_rot180_correct = Vector(-1, -1, 1);
    
    t_rot0 = RotateZ(ang0);
    t_rot90 = RotateZ(ang90);
    t_rot180 = RotateZ(ang180);
    
    // Vectors should be unchanged by rotation = 0
    Vector v0_t = t_rot0(v0);
    Vector v1_t = t_rot0(v1);
    TS_ASSERT_EQUALS(v0_t, v0);
    TS_ASSERT_EQUALS(v1_t, v1);

    v0_t = t_rot90(v0);
    v1_t = t_rot90(v1);
    TS_ASSERT_EQUALS(v0_t, v0);
    TS_ASSERT_DELTA(v1_t.x, v1_rot90_correct.x, 1.0e-5);
    TS_ASSERT_DELTA(v1_t.y, v1_rot90_correct.y, 1.0e-5);
    TS_ASSERT_DELTA(v1_t.z, v1_rot90_correct.z, 1.0e-5);

    v0_t = t_rot180(v0);
    v1_t = t_rot180(v1);
    TS_ASSERT_EQUALS(v0_t, v0);
    TS_ASSERT_DELTA(v1_t.x, v1_rot180_correct.x, 1.0e-5);
    TS_ASSERT_DELTA(v1_t.y, v1_rot180_correct.y, 1.0e-5);
    TS_ASSERT_DELTA(v1_t.z, v1_rot180_correct.z, 1.0e-5);
  }
};
