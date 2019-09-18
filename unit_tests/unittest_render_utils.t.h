// Unit tests for code in render_utils.cpp

#include <cxxtest/TestSuite.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
using namespace std;

#include "utilities_pub.h"
#include "definitions.h"
#include "geometry.h"
#include "render_utils.h"


class NewTestSuite : public CxxTest::TestSuite 
{

  public:
  
  void testSameIOR_simple( void )
  {
    Vector normal1 = Vector(0, 1, 0);
    Vector incident1a = Vector(0, -1.0, 0.0);
    Vector incident1b = Vector(1, 0.0, 0.0);
    Vector normal2 = Vector(-1, 0, 0);
    Vector incident2a = Vector(1, 0.0, 0.0);
    Vector incident2b = Vector(0.7071067811865476, 0.7071067811865476, 0.0);
    Vector outgoing;
    
    outgoing = Refraction(incident1a, normal1, 1.0, 1.0);
    TS_ASSERT_DELTA( outgoing.x, incident1a.x, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.y, incident1a.y, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.z, incident1a.z, 1.0e-6 );
    outgoing = Refraction(incident1b, normal1, 1.0, 1.0);
    TS_ASSERT_DELTA( outgoing.x, incident1b.x, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.y, incident1b.y, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.z, incident1b.z, 1.0e-6 );

    outgoing = Refraction(incident2a, normal2, 1.0, 1.0);
    TS_ASSERT_DELTA( outgoing.x, incident2a.x, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.y, incident2a.y, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.z, incident2a.z, 1.0e-6 );
    outgoing = Refraction(incident2b, normal2, 1.0, 1.0);
    TS_ASSERT_DELTA( outgoing.x, incident2b.x, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.y, incident2b.y, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.z, incident2b.z, 1.0e-6 );
  }

  void testSameIOR_lower_to_higher( void )
  {
    Vector outgoing;
    float  IOR_in = 1.0;
    float  IOR_out = 1.25;

    Vector normal1 = Vector(0, 1, 0);   // pointing straight up
	// incident vector at 90 deg --> transmitted vector at 90 deg
    Vector incident1a = Vector(0, -1.0, 0.0);   // pointing straight down
    outgoing = Refraction(incident1a, normal1, IOR_in, IOR_out);
    TS_ASSERT_DELTA( outgoing.x, incident1a.x, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.y, incident1a.y, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.z, incident1a.z, 1.0e-6 );
	// incident vector at 45 deg w.r.t. normal --> transmitted vector at 34.4499 deg
	// 	outgoing.x = sin(34.4499), outgoing.y = cos(34.4499)
    Vector incident1b = Vector(0.7071067811865476, -0.7071067811865476, 0.0);  // 45 deg
    outgoing = Refraction(incident1b, normal1, IOR_in, IOR_out);
    TS_ASSERT_DELTA( outgoing.x, 0.565685424949238, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.y, -0.8246211251235321, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.z, 0.0, 1.0e-6 );
	// incident vector at -45 deg w.r.t. normal --> transmitted vector at -34.4499 deg
	// 	outgoing.x = sin(34.4499), outgoing.y = cos(34.4499)
    Vector incident1c = Vector(-0.7071067811865476, -0.7071067811865476, 0.0);  // 45 deg
    outgoing = Refraction(incident1c, normal1, IOR_in, IOR_out);
    TS_ASSERT_DELTA( outgoing.x, -0.565685424949238, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.y, -0.8246211251235321, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.z, 0.0, 1.0e-6 );

    Vector normal2 = Vector(-1, 0, 0);   // pointing straight left
	// incident vector at 90 deg --> transmitted vector at 90 deg
    Vector incident2a = Vector(1, 0, 0.0);   // pointing straight right
    outgoing = Refraction(incident2a, normal2, IOR_in, IOR_out);
    TS_ASSERT_DELTA( outgoing.x, incident2a.x, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.y, incident2a.y, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.z, incident2a.z, 1.0e-6 );
	// incident vector at 45 deg w.r.t. normal --> transmitted vector at 34.4499 deg
    Vector incident2b = Vector(0.7071067811865476, -0.7071067811865476, 0.0);  // 45 deg
    outgoing = Refraction(incident2b, normal2, IOR_in, IOR_out);
    TS_ASSERT_DELTA( outgoing.x, 0.8246211251235321, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.y, -0.565685424949238, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.z, 0.0, 1.0e-6 );
	// incident vector at -45 deg w.r.t. normal --> transmitted vector at -34.4499 deg
    Vector incident2c = Vector(0.7071067811865476, 0.7071067811865476, 0.0);  // 45 deg
    outgoing = Refraction(incident2c, normal2, IOR_in, IOR_out);
    TS_ASSERT_DELTA( outgoing.x, 0.8246211251235321, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.y, 0.565685424949238, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.z, 0.0, 1.0e-6 );
  }

  void testSameIOR_higher_to_lower( void )
  {
    Vector outgoing;
    float  IOR_in = 1.25;
    float  IOR_out = 1.0;

    Vector normal1 = Vector(0, 1, 0);   // pointing straight up
	// incident vector at 90 deg --> transmitted vector at 90 deg
    Vector incident1a = Vector(0, -1.0, 0.0);   // pointing straight down
    outgoing = Refraction(incident1a, normal1, IOR_in, IOR_out);
    TS_ASSERT_DELTA( outgoing.x, incident1a.x, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.y, incident1a.y, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.z, incident1a.z, 1.0e-6 );
	// incident vector at 34.4499 deg w.r.t. normal --> transmitted vector at 45 deg
    Vector incident1b = Vector(0.565685424949238, -0.8246211251235321, 0.0);  // 45 deg
    outgoing = Refraction(incident1b, normal1, IOR_in, IOR_out);
    TS_ASSERT_DELTA( outgoing.x, 0.7071067811865476, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.y, -0.7071067811865476, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.z, 0.0, 1.0e-6 );
	// incident vector at -34.4499 deg w.r.t. normal --> transmitted vector at -45 deg
    Vector incident1c = Vector(-0.565685424949238, -0.8246211251235321, 0.0);  // 45 deg
    outgoing = Refraction(incident1c, normal1, IOR_in, IOR_out);
    TS_ASSERT_DELTA( outgoing.x, -0.7071067811865476, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.y, -0.7071067811865476, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.z, 0.0, 1.0e-6 );

    Vector normal2 = Vector(-1, 0, 0);   // pointing straight left
	// incident vector at 90 deg --> transmitted vector at 90 deg
    Vector incident2a = Vector(1, 0, 0.0);   // pointing straight right
    outgoing = Refraction(incident2a, normal2, IOR_in, IOR_out);
    TS_ASSERT_DELTA( outgoing.x, incident2a.x, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.y, incident2a.y, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.z, incident2a.z, 1.0e-6 );
	// incident vector at 34.4499 deg w.r.t. normal --> transmitted vector at 45 deg
    Vector incident2b = Vector(0.8246211251235321, -0.565685424949238, 0.0);  // 45 deg
    outgoing = Refraction(incident2b, normal2, IOR_in, IOR_out);
    TS_ASSERT_DELTA( outgoing.x, 0.7071067811865476, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.y, -0.7071067811865476, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.z, 0.0, 1.0e-6 );
	// incident vector at -45 deg w.r.t. normal --> transmitted vector at -34.4499 deg
    Vector incident2c = Vector(0.8246211251235321, 0.565685424949238, 0.0);  // 45 deg
    outgoing = Refraction(incident2c, normal2, IOR_in, IOR_out);
    TS_ASSERT_DELTA( outgoing.x, 0.7071067811865476, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.y, 0.7071067811865476, 1.0e-6 );
    TS_ASSERT_DELTA( outgoing.z, 0.0, 1.0e-6 );
  }

};
