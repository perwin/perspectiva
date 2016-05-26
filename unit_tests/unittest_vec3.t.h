// Unit tests for code in vec3.h

#include <cxxtest/TestSuite.h>
#include "vec3.h"




class NewTestSuite : public CxxTest::TestSuite 
{
public:

  // Tests for Sphere class
  
  void testEquality( void )
  {
    Vec3f v1 = Vec3f(1.0, 1.0, 1.0);
    Vec3f v2 = Vec3f(1.0, 1.0, 1.0);
  
    TS_ASSERT_EQUALS( (v1 == v2), true);
  }

  void testInquality( void )
  {
    Vec3f v1 = Vec3f(1.0, 1.0, 1.0);
    Vec3f v2a = Vec3f(0.0, 1.0, 1.0);
    Vec3f v2b = Vec3f(1.0, 0.0, 1.0);
    Vec3f v2c = Vec3f(1.0, 1.0, 0.0);
  
    TS_ASSERT_EQUALS( (v1 == v2a), false);
    TS_ASSERT_EQUALS( (v1 == v2b), false);
    TS_ASSERT_EQUALS( (v1 == v2c), false);

    TS_ASSERT_EQUALS( (v1 != v2a), true);
    TS_ASSERT_EQUALS( (v1 != v2b), true);
    TS_ASSERT_EQUALS( (v1 != v2c), true);
  }

};
