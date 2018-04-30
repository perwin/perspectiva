// Unit tests for code in geometry.h

#include <stdio.h>
#include <cxxtest/TestSuite.h>
#include "geometry.h"


// Vector class

class TestVectorClass : public CxxTest::TestSuite 
{
public:

  Vector vector0, vector1;
  
  void setUp( )
  {
    vector0 = Vector(0);
    vector1 = Vector(1.0);
  }
  
  void testVector_Creation( void )
  {
    Vector vector10 = Vector(10.0, 20.0, 30.0);

    TS_ASSERT_DELTA( vector0[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( vector0[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( vector0[2], 0.0, 1.0e-6 );

    TS_ASSERT_DELTA( vector1[0], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( vector1[1], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( vector1[2], 1.0, 1.0e-6 );

    TS_ASSERT_DELTA( vector10[0], 10.0, 1.0e-6 );
    TS_ASSERT_DELTA( vector10[1], 20.0, 1.0e-6 );
    TS_ASSERT_DELTA( vector10[2], 30.0, 1.0e-6 );
  }

  void testVector_Equality( void )
  {
    Vector vector0v2 = Vector(0.0, 0.0, 0.0);

    TS_ASSERT_EQUALS( vector0 == vector0v2, true );
    TS_ASSERT_EQUALS( vector0 != vector0v2, false );
    TS_ASSERT_EQUALS( vector0 == vector1, false );
    TS_ASSERT_EQUALS( vector0 != vector1, true );
  }

  void testVector_Arithmetic_addition( void )
  {
    Vector vector2 = Vector(10.0, 20.0, 30.0);
    Vector vector22 = Vector(10.0, 20.0, 30.0);
    
    Vector vector0_plus_0 = vector0 + vector0;
    Vector vector0_plus_1 = vector0 + vector1;
    Vector vector1_plus_2 = vector1 + vector2;
    Vector vector_1plus2_correct = Vector(11.0, 21.0, 31.0);
    vector22 += vector2;
    Vector vector22_correct = Vector(20.0, 40.0, 60.0);

    TS_ASSERT_EQUALS( vector0_plus_0, vector0 );
    TS_ASSERT_EQUALS( vector0_plus_1, vector1 );
    TS_ASSERT_EQUALS( vector1_plus_2, vector_1plus2_correct );
    TS_ASSERT_EQUALS( vector22, vector22_correct );
  }

  void testVector_Arithmetic_subtraction( void )
  {
    Vector vector2 = Vector(10.0, 20.0, 30.0);
    Vector vector22 = Vector(10.0, 20.0, 30.0);
    
    Vector vector0_minus_0 = vector0 - vector0;
    Vector vector1_minus_0 = vector1 - vector0;
    Vector vector0_minus_1 = vector0 - vector1;
    vector22 -= vector2;

    TS_ASSERT_EQUALS( vector0_minus_0, vector0 );
    TS_ASSERT_EQUALS( vector1_minus_0, vector1 );
    TS_ASSERT_EQUALS( vector0_minus_1, Vector(-1) );
    TS_ASSERT_EQUALS( vector22, vector0 );
  }

  void testVector_Arithmetic_scaling( void )
  {
    Vector vector2 = Vector(1.0, -2.0, 3.0);
    float s10 = 10.0f;
    
    Vector vector0_mult10 = s10*vector0;
    Vector vector1_mult10 = vector1*s10;
    vector2 *= s10;

    TS_ASSERT_EQUALS( vector0_mult10, vector0 );
    TS_ASSERT_EQUALS( vector1_mult10, Vector(10.0) );
    TS_ASSERT_EQUALS( vector2, Vector(10.0, -20.0, 30.0) );
  }

  void testVector_Arithmetic_scaling_by_division( void )
  {
    Vector vector2 = Vector(1.0, -2.0, 3.0);
    float s10 = 10.0f;
    
    Vector vector0_div10 = vector0 / s10;
    Vector vector1_div10 = vector1 / s10;
    vector2 /= s10;

    TS_ASSERT_EQUALS( vector0_div10, vector0 );
    TS_ASSERT_EQUALS( vector1_div10, Vector(0.1) );
    TS_ASSERT_EQUALS( vector2, Vector(0.1, -0.2, 0.3) );
  }

  void testVector_negation( void )
  {
    Vector vector2 = Vector(1.0, -2.0, 3.0);
    
    Vector vector0_neg = -vector0;
    Vector vector1_neg = -vector1;
    Vector vector2_neg = -vector2;

    TS_ASSERT_EQUALS( vector0_neg, vector0 );
    TS_ASSERT_EQUALS( vector1_neg, Vector(-1.0) );
    TS_ASSERT_EQUALS( vector2_neg, Vector(-1.0, 2.0, -3.0) );
  }

};


// Point class

class TestPointClass : public CxxTest::TestSuite 
{
public:
  
  void testPoint_Creation( void )
  {
    Point point0;
    Point point1 = Point(10.0, 20.0, 30.0);
    Point point2 = Point(1.0);

    TS_ASSERT_DELTA( point0[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( point0[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( point0[2], 0.0, 1.0e-6 );

    TS_ASSERT_DELTA( point1[0], 10.0, 1.0e-6 );
    TS_ASSERT_DELTA( point1[1], 20.0, 1.0e-6 );
    TS_ASSERT_DELTA( point1[2], 30.0, 1.0e-6 );

    TS_ASSERT_DELTA( point2[0], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( point2[1], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( point2[2], 1.0, 1.0e-6 );
  }

  void testPoint_Equality( void )
  {
    Point point0;
    Point point1 = Point(0.0, 0.0, 0.0);
    Point point2 = Point(1.0);

    TS_ASSERT_EQUALS( point0 == point1, true );
    TS_ASSERT_EQUALS( point0 != point1, false );
    TS_ASSERT_EQUALS( point0 == point2, false );
    TS_ASSERT_EQUALS( point0 != point2, true );
  }

  void testPoint_Subtraction( void )
  {
    Point p0 = Point(0.0);
    Point p1 = Point(1.0);

    Vector v_p0top1 = p1 - p0;
    Vector v_p1top0 = p0 - p1;
    Vector v_p0top1_correct = Vector(1.0);
    TS_ASSERT_EQUALS( v_p0top1, v_p0top1_correct );
    TS_ASSERT_EQUALS( v_p1top0, -v_p0top1_correct );
//     TS_ASSERT_DELTA( v_p0top1[0], v_p0top1_correct[0], 1.0e-6 );
//     TS_ASSERT_DELTA( v_p0top1[1], v_p0top1_correct[1], 1.0e-6 );
//     TS_ASSERT_DELTA( v_p0top1[2], v_p0top1_correct[2], 1.0e-6 );
  }

  void testPoint_VectorAddition( void )
  {
    Point p0 = Point(0.0);
    Point p1 = Point(1.0);
    Point p2 = Point(1.0);
    Vector v1 = Vector(1.0);
    Vector v2 = Vector(0.0, -10.0, 2.0);

    Point p0_plus_v1 = p0 + v1;
    Point p1_plus_v2 = p1 + v2;
    p2 += v1;
    
    TS_ASSERT_EQUALS( p0_plus_v1, p1 );
    TS_ASSERT_EQUALS( p1_plus_v2, Point(1.0, -9.0, 3.0) );
    TS_ASSERT_EQUALS( p2, Point(2.0) );
  }

  void testPoint_VectorSubtraction( void )
  {
    Point p0 = Point(0.0);
    Point p1 = Point(1.0);
    Point p2 = Point(1.0);
    Vector v1 = Vector(1.0);
    Vector v2 = Vector(0.0, -10.0, 2.0);

    Point p0_minus_v1 = p0 - v1;
    Point p1_minus_v2 = p1 - v2;
    p2 -= v1;
    
    TS_ASSERT_EQUALS( p0_minus_v1, Point(-1.0) );
    TS_ASSERT_EQUALS( p1_minus_v2, Point(1.0, 11.0, -1.0) );
    TS_ASSERT_EQUALS( p2, Point(0.0) );
  }
};


// Ray class

class TestRayClass : public CxxTest::TestSuite 
{
public:

  Ray ray1, ray2;

  void testRay_Creation( void )
  {
    Point p0 = Point(0);
    Vector v1 = Vector(1.0);
    
    ray1 = Ray(p0, v1); 
    ray2 = Ray(p0, v1, 2, 1.5); 
    
    TS_ASSERT_EQUALS( ray1.o, p0 );
    TS_ASSERT_EQUALS( ray1.dir, v1 );
    TS_ASSERT_EQUALS( ray1.depth, 1 );
    TS_ASSERT_EQUALS( ray1.currentIOR, 1.0 );

    TS_ASSERT_EQUALS( ray2.o, p0 );
    TS_ASSERT_EQUALS( ray2.dir, v1 );
    TS_ASSERT_EQUALS( ray2.depth, 2 );
    TS_ASSERT_EQUALS( ray2.currentIOR, 1.5 );
  }

  void testRay_Creation_fromPoints( void )
  {
    Point p0 = Point(0);
    Point p1 = Point(10.0);
    Vector v1 = Vector(10.0);
    
    ray1 = Ray(p0, p1); 
    
    TS_ASSERT_EQUALS( ray1.o, p0 );
    TS_ASSERT_EQUALS( ray1.dir, v1 );
    TS_ASSERT_EQUALS( ray1.depth, 1 );
    TS_ASSERT_EQUALS( ray1.currentIOR, 1.0 );
  }

  void testRay_func_of_t( void )
  {
    Point p0 = Point(0);
    Vector v1 = Vector(1.0);
    
    ray1 = Ray(p0, v1); 
    
    Point p_t = ray1(10.0);
    Point p_t_correct = Point(10.0);
    
    TS_ASSERT_EQUALS( p_t, p_t_correct );
  }

};
