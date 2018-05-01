// Unit tests for code in transform.h

#include <stdio.h>
#include <cxxtest/TestSuite.h>

#include "geometry.h"
#include "transform.h"

// Transform class

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
