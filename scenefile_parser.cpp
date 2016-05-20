// Code for parsing scene files

#include <stdio.h>
#include <string>
#include <vector>
#include "yaml-cpp/yaml.h"
#include "vec3.h"
#include "geometry.h"
#include "scene.h"
#include "scenefile_parser.h"


bool VetSceneFile( const std::string sceneFilename )
{
  return false;
}


float GetFileVersion( const std::string sceneFilename )
{
  float  versionNum = -1.0;
  YAML::Node sceneFile = YAML::LoadFile(sceneFilename.c_str());
  if (sceneFile["scenefile_version"])
  	versionNum = sceneFile["scenefile_version"].as<float>();
  return versionNum;
}


void AddSphereToScene( YAML::Node sphereNode, Scene *theScene, int debugLevel )
{
  float  x, y, z, radius, reflec, transp;
  float  r, g, b, e_r, e_g, e_b;
  bool  isLight = false;
  
  YAML::Node pos = sphereNode["position"];
  x = pos[0].as<float>();
  y = pos[1].as<float>();
  z = pos[2].as<float>();
  if (debugLevel > 0)
    printf("   sphere with position = %f, %f, %f\n", x, y, z);
  radius = sphereNode["radius"].as<float>();
  reflec = sphereNode["reflectivity"].as<float>();
  transp = sphereNode["transparency"].as<float>();
  if (debugLevel > 0)
    printf("      r = %f, reflec = %f, transparency = %f\n", radius, reflec, transp);
  YAML::Node surfColor = sphereNode["surface_color"];
  r = surfColor[0].as<float>();
  g = surfColor[1].as<float>();
  b = surfColor[2].as<float>();
  if (debugLevel > 0)
    printf("      surface_color = %f, %f, %f\n", r,g,b);
  if (sphereNode["emissivity"]) {
    YAML::Node emissivity = sphereNode["emissivity"];
    e_r = emissivity[0].as<float>();
    e_g = emissivity[1].as<float>();
    e_b = emissivity[2].as<float>();
    if (debugLevel > 0)
      printf("      emissivity = %f, %f, %f\n", e_r,e_g,e_b);
    if ((e_r > 0.0) || (e_g > 0.0) || (e_b > 0.0))
      isLight = true;
  }
  if (isLight)
    theScene->AddSphere(Vec3f(x,y,z), radius, Vec3f(r,g,b), reflec, transp,
    						Vec3f(e_r,e_g,e_b));
  else
    theScene->AddSphere(Vec3f(x,y,z), radius, Vec3f(r,g,b), reflec, transp);
}


/// Allocates and returns a Scene object
Scene* LoadSceneFromFile( std::string sceneFilename, int debugLevel )
{
  int  nObjects;
  std::vector<Sphere> objects;
  Scene *theScene = new Scene();
    
  YAML::Node sceneFile = YAML::LoadFile(sceneFilename.c_str());
  
  if (sceneFile["scene"]) {
    nObjects = (int)sceneFile["scene"].size();
    if (debugLevel > 0)
      printf("Scene detected with %d objects.\n", nObjects);
    for (int i = 0; i < nObjects; ++i) {
      YAML::Node thisObject = sceneFile["scene"][i];
      if (thisObject["sphere"])
        AddSphereToScene(thisObject["sphere"], theScene);
    }
  }
  
  if (debugLevel > 0)
    printf("%d spheres found and stored in vector\n", (int)objects.size());
  return theScene;
}
