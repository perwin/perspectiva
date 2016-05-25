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
#include "definitions.h"
#include "vec3.h"
#include "color.h"
#include "geometry.h"
#include "scene.h"
#include "scenefile_parser.h"

const string  TEST_SCENEFILE_GOOD("tests/scene_with_light_plane.yml");
const string  TEST_SCENEFILE_GOOD2("tests/scene_good_oddball.yml");
const string  TEST_SCENEFILE_BAD1("tests/badscene.yml");

const float  CURRENT_SCENFILE_VERSION = 0.3;


class NewTestSuite : public CxxTest::TestSuite 
{
public:
  Scene *scene1;
  Scene *scene2;
  YAML::Node sceneFile1, sceneFile2;
  YAML::Node sphereNode, planeNode, lightNode, backgroundNode, iorNode;
  
  
  void setUp()
  {
    scene1 = new Scene();
    scene2 = new Scene();
    
    sceneFile1 = YAML::LoadFile(TEST_SCENEFILE_GOOD.c_str());
    sphereNode = sceneFile1["scene"][0]["sphere"];
    planeNode = sceneFile1["scene"][4]["plane"];
    lightNode = sceneFile1["scene"][5]["light"];
    backgroundNode = sceneFile1["scene"][6]["background"];

    sceneFile2 = YAML::LoadFile(TEST_SCENEFILE_GOOD2.c_str());
    iorNode = sceneFile2["scene"][5]["atmosphere"];
  }

  void tearDown()
  {
    delete scene1;
    delete scene2;
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
    float  correct = CURRENT_SCENFILE_VERSION;
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
  //     - sphere:
  //         position: [0.0, 0.0, -20.0]
  //         radius: 4.0
  //         surface_color: [1.00, 0.32, 0.36]
  //         reflectivity: 1.0
  //         transparency: 0.5

  void testGetSphere( void )
  {
    AddSphereToScene(sphereNode, scene1);
    // sphere should now be first object in scene1's object vector
    Sphere *thisSphere = (Sphere *)scene1->objects[0];
    
    TS_ASSERT_DELTA( thisSphere->center[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->center[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->center[2], -20.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->radius, 4.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->radius2, 16.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->surfaceColor[0], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->surfaceColor[1], 0.32, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->surfaceColor[2], 0.36, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->reflection, 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->transparency, 0.5, 1.0e-6 );
  }

  // Test reading and storing a plane
  //     - plane:
  //         position: [0.0, -15.0, -15.0]
  //         normal: [0.0, 1.0, 0.0]
  //         surface_color: [0.90, 0.90, 0.90]
  //         reflectivity: 1.0
  //         transparency: 0.0

  void testGetPlane( void )
  {
    AddPlaneToScene(planeNode, scene2);
    // plane should now be first object in scene2's object vector
    Plane *thisPlane = (Plane *)scene2->objects[0];
    
    TS_ASSERT_DELTA( thisPlane->center[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->center[1], -15.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->center[2], -15.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->norm[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->norm[1], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->norm[2], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->surfaceColor[0], 0.9, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->surfaceColor[1], 0.9, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->surfaceColor[2], 0.9, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->reflection, 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->transparency, 0.0, 1.0e-6 );
  }

  // Test reading and storing a point light -- NOT IMPLEMENTED YET!
  //     - light:
  //         type: point
  //         position: [0.0, 20.0, -30.0]
  //         luminosity: 3.0
  //         color: [0.1, 0.1, 0.1]

  void testGetLight( void )
  {
    AddLightToScene(lightNode, scene1);
    PointLight *thisLight = (PointLight *)scene1->lights[0];
    
    TS_ASSERT_EQUALS( thisLight->lightType, LIGHT_POINT );
    TS_ASSERT_DELTA( thisLight->position[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->position[1], 20.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->position[2], -30.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[0], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[1], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[2], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->luminosity, 3.0, 1.0e-6 );
  }

  void testSetBackground( void )
  {
    AddBackgroundToScene(backgroundNode, scene1);
    
    TS_ASSERT_DELTA( scene1->backgroundColor[0], 2.0, 1.0e-6 );
    TS_ASSERT_DELTA( scene1->backgroundColor[1], 2.0, 1.0e-6 );
    TS_ASSERT_DELTA( scene1->backgroundColor[2], 2.0, 1.0e-6 );
  }

  void testSetIOR( void )
  {
    TS_ASSERT_DELTA( scene1->defaultIOR, 1.0, 1.0e-6 );
    AddAtmosphereToScene(iorNode, scene1);
    
    TS_ASSERT_DELTA( scene1->defaultIOR, 1.5, 1.0e-6 );
  }


  void testReadEntireScene( void )
  {
    Scene *newScene;
    
    newScene = LoadSceneFromFile(TEST_SCENEFILE_GOOD);
    
    TS_ASSERT_EQUALS( newScene->objects.size(), 5);
    TS_ASSERT_EQUALS( newScene->lights.size(), 1);

    TS_ASSERT_DELTA( newScene->backgroundColor[0], 2.0, 1.0e-6 );
    TS_ASSERT_DELTA( newScene->backgroundColor[1], 2.0, 1.0e-6 );
    TS_ASSERT_DELTA( newScene->backgroundColor[2], 2.0, 1.0e-6 );

    PointLight *thisLight = (PointLight *)newScene->lights[0];
    TS_ASSERT_EQUALS( thisLight->lightType, LIGHT_POINT );
    TS_ASSERT_DELTA( thisLight->position[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->position[1], 20.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->position[2], -30.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[0], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[1], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[2], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->luminosity, 3.0, 1.0e-6 );

    // check the first object (sphere)
    Sphere *thisSphere = (Sphere *)newScene->objects[0];
    TS_ASSERT_DELTA( thisSphere->center[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->center[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->center[2], -20.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->radius, 4.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->radius2, 16.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->surfaceColor[0], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->surfaceColor[1], 0.32, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->surfaceColor[2], 0.36, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->reflection, 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->transparency, 0.5, 1.0e-6 );

    // check the last object (plane)
    Plane *thisPlane = (Plane *)newScene->objects[4];   
    TS_ASSERT_DELTA( thisPlane->center[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->center[1], -15.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->center[2], -15.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->norm[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->norm[1], 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->norm[2], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->surfaceColor[0], 0.9, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->surfaceColor[1], 0.9, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->surfaceColor[2], 0.9, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->reflection, 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisPlane->transparency, 0.0, 1.0e-6 );

    delete newScene;
  }

};
