// Unit tests for code in scenefile_parser.cpp

// older compilation notes:
// $ cxxtestgen.py --error-printer -o test_runner.cpp unit_tests/unittest_scenefile_parser.t.h
// $ clang++ -o test_runner test_runner.cpp utilities.cpp scenefile_parser.cpp -I/usr/local/include -lyaml-cpp

#include <cxxtest/TestSuite.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
using namespace std;

#include "utilities_pub.h"
#include "definitions.h"
#include "vec3.h"
#include "color.h"
#include "geometry.h"
#include "materials.h"
#include "cameras.h"
#include "scene.h"
#include "scenefile_parser.h"

const string  TEST_SCENEFILE_GOOD("tests/scene_with_light_plane.yml");
const string  TEST_SCENEFILE_GOOD_WITH_MATERIALS("tests/scene_with_light_plane_and_materials.yml");
const string  TEST_SCENEFILE_GOOD2("tests/scene_distantlight.yml");
const string  TEST_SCENEFILE_GOOD3("tests/scene_good_oddball.yml");
const string  TEST_SCENEFILE_GOOD4("tests/scene_arealights.yml");
const string  TEST_SCENEFILE_GOOD5("tests/scene_cameratest.yml");
const string  TEST_SCENEFILE_GOOD6("tests/scene_materialtest.yml");
const string  TEST_SCENEFILE_BAD1("tests/badscene.yml");

const float  CURRENT_SCENFILE_VERSION = 0.3;


class NewTestSuite : public CxxTest::TestSuite 
{
public:
  Scene *scene1;
  Scene *scene2;
  Scene *scene3;
  Scene *scene4;
  Scene *scene5;
  Scene *scene6;   // for testing adding camera
  Scene *scene7;   // for testing material
  YAML::Node sceneFile1, sceneFile2, sceneFile3, sceneFile4, sceneFile5, sceneFile6;
  YAML::Node sphereNode, planeNode;
  YAML::Node pointLightNode, distantLightNode, sphericalLightNode, rectLightNode;
  YAML::Node cameraNode;
  YAML::Node materialNode_PlainRed, materialNode_GoldMirror;
  YAML::Node backgroundNode, iorNode;
  
  
  void setUp()
  {
    scene1 = new Scene();
    scene2 = new Scene();
    scene3 = new Scene();
    scene4 = new Scene();
    scene5 = new Scene();
    scene6 = new Scene();
    scene7 = new Scene();
    
    sceneFile1 = YAML::LoadFile(TEST_SCENEFILE_GOOD.c_str());
    sphereNode = sceneFile1["scene"][0]["sphere"];
    planeNode = sceneFile1["scene"][4]["plane"];
    pointLightNode = sceneFile1["scene"][5]["light"];
    backgroundNode = sceneFile1["scene"][6]["background"];

    sceneFile2 = YAML::LoadFile(TEST_SCENEFILE_GOOD2.c_str());
    distantLightNode = sceneFile2["scene"][2]["light"];

    sceneFile3 = YAML::LoadFile(TEST_SCENEFILE_GOOD3.c_str());
    iorNode = sceneFile3["scene"][5]["atmosphere"];

    sceneFile4 = YAML::LoadFile(TEST_SCENEFILE_GOOD4.c_str());
    sphericalLightNode = sceneFile4["scene"][2]["light"];
    rectLightNode = sceneFile4["scene"][3]["light"];

    sceneFile5 = YAML::LoadFile(TEST_SCENEFILE_GOOD5.c_str());
    cameraNode = sceneFile5["scene"][4]["camera"];

    sceneFile6 = YAML::LoadFile(TEST_SCENEFILE_GOOD6.c_str());
    materialNode_PlainRed = sceneFile6["scene"][0]["material"];
    materialNode_GoldMirror = sceneFile6["scene"][1]["material"];
  }

  void tearDown()
  {
    delete scene1;
    delete scene2;
    delete scene3;
    delete scene4;
    delete scene5;
    delete scene6;
    delete scene7;
  }


  // Test good scene file
//   void testVetSceneFile_Simple( void )
//   {
//     
//     bool fileStatus = VetSceneFile(TEST_SCENEFILE_GOOD);
//     TS_ASSERT( fileStatus == true );
//   }


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
    // sphere should now be first shape in scene1's shape vector
    Sphere *thisSphere = (Sphere *)scene1->shapes[0];
    
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
    // plane should now be first shape in scene2's shape vector
    Plane *thisPlane = (Plane *)scene2->shapes[0];
    
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

  // Test reading and storing a point light
  //     - light:
  //         type: point
  //         position: [0.0, 20.0, -30.0]
  //         luminosity: 3.0
  //         color: [0.1, 0.1, 0.1]

  void testGetPointLight( void )
  {
    AddLightToScene(pointLightNode, scene1);
    PointLight *thisLight = (PointLight *)scene1->lights[0];
    
    TS_ASSERT_EQUALS( thisLight->lightType, LIGHT_POINT );
    TS_ASSERT_DELTA( thisLight->lightPosition[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightPosition[1], 20.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightPosition[2], -30.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[0], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[1], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[2], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->luminosity, 3.0, 1.0e-6 );
  }

  void testGetDistantLight( void )
  {
    float  scaling = 1.0/sqrt(2.0);
    float  correctLum = 1.0;
    
    AddLightToScene(distantLightNode, scene3);
    DistantLight *thisLight = (DistantLight *)scene3->lights[0];
    
    TS_ASSERT_EQUALS( thisLight->lightType, LIGHT_DISTANT );
    TS_ASSERT_DELTA( thisLight->dir[0], -1.0*scaling, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->dir[1], -1.0*scaling, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->dir[2], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[0], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[1], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[2], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->luminosity, correctLum, 1.0e-6 );
  }

  // Test reading and storing a rectangular light
//     - light:
//         type: rect
//         position: [0.0, 20.0, -30.0]
//         xsize: 10.0
//         zsize: 5.0
//         luminosity: 100.0
//         color: [0.1, 0.1, 0.1]
//         nsamples: 5
  void testGetRectLight( void )
  {
    AddLightToScene(rectLightNode, scene5);
    RectLight *thisLight = (RectLight *)scene5->lights[0];
    
    TS_ASSERT_EQUALS( thisLight->lightType, LIGHT_RECT );
    TS_ASSERT_DELTA( thisLight->lightPosition[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightPosition[1], 20.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightPosition[2], -30.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->xSize, 10.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->zSize, 5.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[0], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[1], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[2], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->luminosity, 100.0, 1.0e-6 );
    TS_ASSERT_EQUALS( thisLight->NSamples(), 5 );
  }

  // Test reading and storing a (visible) spherical light
//     - light:
//         type: sphere
//         position: [0.0, 20.0, -30.0]
//         radius: 5.0
//         luminosity: 100.0
//         color: [0.1, 0.1, 0.1]
//         nsamples: 5
  void testGetSphericalLight( void )
  {
    AddLightToScene(sphericalLightNode, scene4);
    SphericalLight *thisLight = (SphericalLight *)scene4->lights[0];
    Sphere *thisSphere = (Sphere *)scene4->shapes[0];
    
    // spherical light as light
    TS_ASSERT_EQUALS( thisLight->lightType, LIGHT_SPHERE );
    TS_ASSERT_DELTA( thisLight->lightPosition[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightPosition[1], 20.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightPosition[2], -30.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->radius, 5.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[0], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[1], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[2], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->luminosity, 100.0, 1.0e-6 );
    TS_ASSERT_EQUALS( thisLight->NSamples(), 5 );
    
    // spherical light as sphere
    TS_ASSERT_EQUALS( scene4->shapes.size(), 1);
    TS_ASSERT_DELTA( thisSphere->center[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->center[1], 20.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->center[2], -30.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->radius, 5.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->radius2, 25.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->surfaceColor[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->surfaceColor[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->surfaceColor[2], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->emissionColor[0], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->emissionColor[1], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->emissionColor[2], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->reflection, 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisSphere->transparency, 0.0, 1.0e-6 );
  }

  // Test reading and storing a material
  //     - material:
  //         type: SimpleMaterial
  //         name: PlainRed
  //         surfaceColor: [0.5, 0.16, 0.18]
  //         reflectionColor: 0
  //         refractionColor: 0
  //         emissionColor: 0
  //         reflection: 0.0
  //         transparency: 0.0
  void testMaterials( void )
  {
    Color  surfColor, reflectColor, refractColor, emissColor;
    SimpleMaterial *thisMaterial;
    string  matName;
    
    AddMaterialToScene(materialNode_PlainRed, scene7);
    AddMaterialToScene(materialNode_GoldMirror, scene7);
    
    // PlainRed material
    matName = "PlainRed";
    thisMaterial = (SimpleMaterial *)scene7->materials[matName];
    surfColor = thisMaterial->GetSurfaceColor();
    TS_ASSERT_DELTA( surfColor[0], 0.5, 1.0e-6 );
    TS_ASSERT_DELTA( surfColor[1], 0.16, 1.0e-6 );
    TS_ASSERT_DELTA( surfColor[2], 0.18, 1.0e-6 );
    reflectColor = thisMaterial->GetReflectionColor();
    TS_ASSERT_DELTA( reflectColor[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( reflectColor[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( reflectColor[2], 0.0, 1.0e-6 );
    refractColor = thisMaterial->GetRefractionColor();
    TS_ASSERT_DELTA( refractColor[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( refractColor[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( refractColor[2], 0.0, 1.0e-6 );
    emissColor = thisMaterial->GetEmissionColor();
    TS_ASSERT_DELTA( emissColor[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( emissColor[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( emissColor[2], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial->GetReflectivity(), 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial->GetTransparency(), 0.0, 1.0e-6 );
    TS_ASSERT_EQUALS( thisMaterial->HasSpecular(), false );

	// GoldMirror material
    matName = "GoldMirror";
    thisMaterial = (SimpleMaterial *)scene7->materials[matName];
    surfColor = thisMaterial->GetSurfaceColor();
    TS_ASSERT_DELTA( surfColor[0], 0.9, 1.0e-6 );
    TS_ASSERT_DELTA( surfColor[1], 0.76, 1.0e-6 );
    TS_ASSERT_DELTA( surfColor[2], 0.46, 1.0e-6 );
    reflectColor = thisMaterial->GetReflectionColor();
    TS_ASSERT_DELTA( reflectColor[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( reflectColor[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( reflectColor[2], 0.0, 1.0e-6 );
    refractColor = thisMaterial->GetRefractionColor();
    TS_ASSERT_DELTA( refractColor[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( refractColor[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( refractColor[2], 0.0, 1.0e-6 );
    emissColor = thisMaterial->GetEmissionColor();
    TS_ASSERT_DELTA( emissColor[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( emissColor[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( emissColor[2], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial->GetReflectivity(), 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisMaterial->GetTransparency(), 0.0, 1.0e-6 );
    TS_ASSERT_EQUALS( thisMaterial->HasSpecular(), false );
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

  // Test reading and storing a camera
//     - camera:
//         fov: 25
//         width: 500
//         height: 350
  void testGetCamera( void )
  {
    AddCameraToScene(cameraNode, scene6);
    Camera *thisCamera = (Camera *)scene6->camera;
    
    TS_ASSERT_EQUALS( thisCamera->cameraType, 0 );
    TS_ASSERT_DELTA( thisCamera->fieldOfView, 25.0, 1.0e-6 );
  }


  void testReadEntireScene( void )
  {
    Scene *newScene;
    
    newScene = LoadSceneFromFile(TEST_SCENEFILE_GOOD);
    
    TS_ASSERT_EQUALS( newScene->shapes.size(), 5);
    TS_ASSERT_EQUALS( newScene->lights.size(), 1);

    TS_ASSERT_DELTA( newScene->backgroundColor[0], 2.0, 1.0e-6 );
    TS_ASSERT_DELTA( newScene->backgroundColor[1], 2.0, 1.0e-6 );
    TS_ASSERT_DELTA( newScene->backgroundColor[2], 2.0, 1.0e-6 );

    PointLight *thisLight = (PointLight *)newScene->lights[0];
    TS_ASSERT_EQUALS( thisLight->lightType, LIGHT_POINT );
    TS_ASSERT_DELTA( thisLight->lightPosition[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightPosition[1], 20.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightPosition[2], -30.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[0], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[1], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[2], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->luminosity, 3.0, 1.0e-6 );

    // check the first shape (sphere)
    Sphere *thisSphere = (Sphere *)newScene->shapes[0];
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

    // check the last shape (plane)
    Plane *thisPlane = (Plane *)newScene->shapes[4];   
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


  void testReadEntireScene_with_Materials( void )
  {
    Scene *newScene;
    
    newScene = LoadSceneFromFile(TEST_SCENEFILE_GOOD_WITH_MATERIALS);
    
    TS_ASSERT_EQUALS( newScene->shapes.size(), 5);
    TS_ASSERT_EQUALS( newScene->lights.size(), 1);
    TS_ASSERT_EQUALS( newScene->materials.size(), 2);

	// check names of materials
	TS_ASSERT_EQUALS( newScene->materials.count(string("PlainRed")), 1);
	TS_ASSERT_EQUALS( newScene->materials.count(string("GoldMirror")), 1);
	TS_ASSERT_EQUALS( newScene->materials.count(string("nonexistent")), 0);
	// check preliminary assignment of materials
	TS_ASSERT_EQUALS( newScene->materials_for_shapes[0], "**none**");
	TS_ASSERT_EQUALS( newScene->materials_for_shapes[1], "PlainRed");
	TS_ASSERT_EQUALS( newScene->materials_for_shapes[2], "GoldMirror");
	TS_ASSERT_EQUALS( newScene->materials_for_shapes[3], "**none**");
	TS_ASSERT_EQUALS( newScene->materials_for_shapes[4], "**none**");

	// check value of material
//     - material:
//         type: SimpleMaterial
//         name: PlainRed
//         surfaceColor: [0.5, 0.16, 0.18]
//         reflectionColor: [0, 0, 0]
//         refractionColor: [0, 0, 0]
//         emissionColor: [0, 0, 0]
//         reflectivity: 0.0
//         transparency: 0.0
	SimpleMaterial *mat = (SimpleMaterial *)newScene->materials["PlainRed"];
    Color surfColor = mat->GetSurfaceColor();
    TS_ASSERT_DELTA( surfColor[0], 0.5, 1.0e-6 );
    TS_ASSERT_DELTA( surfColor[1], 0.16, 1.0e-6 );
    TS_ASSERT_DELTA( surfColor[2], 0.18, 1.0e-6 );
    Color reflecColor = mat->GetReflectionColor();
    TS_ASSERT_DELTA( reflecColor[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( reflecColor[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( reflecColor[2], 0.0, 1.0e-6 );
    Color refracColor = mat->GetRefractionColor();
    TS_ASSERT_DELTA( refracColor[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( refracColor[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( refracColor[2], 0.0, 1.0e-6 );
    Color emissColor = mat->GetEmissionColor();
    TS_ASSERT_DELTA( emissColor[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( emissColor[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( emissColor[2], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( mat->GetReflectivity(), 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( mat->GetTransparency(), 0.0, 1.0e-6 );

//     - material:
//         type: SimpleMaterial
//         name: GoldMirror
//         surfaceColor: [0.9, 0.76, 0.46]
//         reflectionColor: [0, 0, 0]
//         refractionColor: [0, 0, 0]
//         emissionColor: [0, 0, 0]
//         reflectivity: 1.0
//         transparency: 0.0
	mat = (SimpleMaterial *)newScene->materials["GoldMirror"];
    surfColor = mat->GetSurfaceColor();
    TS_ASSERT_DELTA( surfColor[0], 0.9, 1.0e-6 );
    TS_ASSERT_DELTA( surfColor[1], 0.76, 1.0e-6 );
    TS_ASSERT_DELTA( surfColor[2], 0.46, 1.0e-6 );
    reflecColor = mat->GetReflectionColor();
    TS_ASSERT_DELTA( reflecColor[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( reflecColor[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( reflecColor[2], 0.0, 1.0e-6 );
    refracColor = mat->GetRefractionColor();
    TS_ASSERT_DELTA( refracColor[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( refracColor[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( refracColor[2], 0.0, 1.0e-6 );
    emissColor = mat->GetEmissionColor();
    TS_ASSERT_DELTA( emissColor[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( emissColor[1], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( emissColor[2], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( mat->GetReflectivity(), 1.0, 1.0e-6 );
    TS_ASSERT_DELTA( mat->GetTransparency(), 0.0, 1.0e-6 );


	// Check that everything else was read in correctly
	
    TS_ASSERT_DELTA( newScene->backgroundColor[0], 2.0, 1.0e-6 );
    TS_ASSERT_DELTA( newScene->backgroundColor[1], 2.0, 1.0e-6 );
    TS_ASSERT_DELTA( newScene->backgroundColor[2], 2.0, 1.0e-6 );

    PointLight *thisLight = (PointLight *)newScene->lights[0];
    TS_ASSERT_EQUALS( thisLight->lightType, LIGHT_POINT );
    TS_ASSERT_DELTA( thisLight->lightPosition[0], 0.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightPosition[1], 20.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightPosition[2], -30.0, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[0], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[1], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->lightColor[2], 0.1, 1.0e-6 );
    TS_ASSERT_DELTA( thisLight->luminosity, 3.0, 1.0e-6 );

    // check the first shape (sphere)
    Sphere *thisSphere = (Sphere *)newScene->shapes[0];
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

    // check the last shape (plane)
    Plane *thisPlane = (Plane *)newScene->shapes[4];   
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
