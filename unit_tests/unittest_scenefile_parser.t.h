// Unit tests for code in scenefile_parser.cpp

// older compilation notes:
// $ cxxtestgen.py --error-printer -o test_runner.cpp unit_tests/unittest_scenefile_parser.t.h
// $ clang++ -o test_runner test_runner.cpp utilities.cpp scenefile_parser.cpp -I/usr/local/include -lyaml-cpp

#include <cxxtest/TestSuite.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#include "utilities_pub.h"
#include "vec3.h"
#include "geometry.h"
#include "scene.h"
#include "scenefile_parser.h"

const string  TEST_SCENEFILE_GOOD("tests/simplescene.yml");
const string  TEST_SCENEFILE_BAD1("tests/badscene.yml");


class NewTestSuite : public CxxTest::TestSuite 
{
public:
  Scene *scene1;
  YAML::Node sceneFile1;
  YAML::Node sphereNode;
  
  
  void setUp()
  {
    scene1 = new Scene();
    sceneFile1 = YAML::LoadFile(TEST_SCENEFILE_GOOD.c_str());
    sphereNode = sceneFile1["scene"][0]["sphere"];
  }

  void tearDown()
  {
    delete scene1;
  }


  // Test good scene file
  void testVetSceneFile_Simple( void )
  {
    
    bool fileStatus = VetSceneFile(TEST_SCENEFILE_GOOD);
    TS_ASSERT( fileStatus == true );
  }


  // Test getting version number
  void testGetVersionNumber_good( void )
  {
    float  correct = 1.0;
    float  versionNum = GetFileVersion(TEST_SCENEFILE_GOOD);
    TS_ASSERT_EQUALS(versionNum, correct);
  }

  void testGetVersionNumber_bad( void )
  {
    float  correct = -1.0;
    float  versionNum = GetFileVersion(TEST_SCENEFILE_BAD1);
    TS_ASSERT_EQUALS(versionNum, correct);
  }


  // Test reading and storing a sphere
  void testGetSphere( void )
  {
    AddSphereToScene(sphereNode, scene1);
    Sphere thisSphere = scene1->objects[0];
    
    TS_ASSERT_DELTA( thisSphere.center[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.center[1], -10004.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.center[2], -20.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.radius, 10000.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.radius2, 100000000.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.surfaceColor[0], 0.2, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.surfaceColor[1], 0.2, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.surfaceColor[2], 0.2, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.transparency, 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere.reflection, 0.0, 1.0e-6 );
  }


//     - sphere:
//         position: [0.0, -10004.0, -20.0]
//         radius: 10000.0
//         surface_color: [0.20, 0.20, 0.20]
//         reflectivity: 0.0
//         transparency: 0.0
// 
// public: 
//   Vec3f center;                           /// position of the sphere 
//   float radius, radius2;                  /// sphere radius and radius^2 
//   Vec3f surfaceColor, emissionColor;      /// surface color and emission (light) 
//   float transparency, reflection;         /// surface transparency and reflectivity 


};
