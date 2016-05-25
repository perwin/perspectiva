// Code for parsing scene files

#include <stdio.h>
#include <string>
#include <vector>
#include "yaml-cpp/yaml.h"
#include "vec3.h"
#include "color.h"
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
    theScene->AddSphere(Vec3f(x,y,z), radius, Color(r,g,b), reflec, transp,
    						Color(e_r,e_g,e_b));
  else
    theScene->AddSphere(Vec3f(x,y,z), radius, Color(r,g,b), reflec, transp);
}


void AddPlaneToScene( YAML::Node objNode, Scene *theScene, int debugLevel )
{
  float  x, y, z, n_x, n_y, n_z, reflec, transp;
  float  r, g, b, e_r, e_g, e_b;
  bool  isLight = false;
  
  YAML::Node pos = objNode["position"];
  x = pos[0].as<float>();
  y = pos[1].as<float>();
  z = pos[2].as<float>();
  if (debugLevel > 0)
    printf("   plane with position = %f, %f, %f\n", x, y, z);
  YAML::Node norm = objNode["normal"];
  n_x = norm[0].as<float>();
  n_y = norm[1].as<float>();
  n_z = norm[2].as<float>();
  if (debugLevel > 0)
    printf("   plane with normal = %f, %f, %f\n", n_x, n_y, n_z);
  reflec = objNode["reflectivity"].as<float>();
  transp = objNode["transparency"].as<float>();
  if (debugLevel > 0)
    printf("      reflec = %f, transparency = %f\n", reflec, transp);
  YAML::Node surfColor = objNode["surface_color"];
  r = surfColor[0].as<float>();
  g = surfColor[1].as<float>();
  b = surfColor[2].as<float>();
  if (debugLevel > 0)
    printf("      surface_color = %f, %f, %f\n", r,g,b);
  if (objNode["emissivity"]) {
    YAML::Node emissivity = objNode["emissivity"];
    e_r = emissivity[0].as<float>();
    e_g = emissivity[1].as<float>();
    e_b = emissivity[2].as<float>();
    if (debugLevel > 0)
      printf("      emissivity = %f, %f, %f\n", e_r,e_g,e_b);
    if ((e_r > 0.0) || (e_g > 0.0) || (e_b > 0.0))
      isLight = true;
  }
  if (isLight)
    theScene->AddPlane(Vec3f(x,y,z), Vec3f(n_x,n_y,n_z), Color(r,g,b), reflec, transp,
    						Color(e_r,e_g,e_b));
  else
    theScene->AddPlane(Vec3f(x,y,z), Vec3f(n_x,n_y,n_z), Color(r,g,b), reflec, transp);
}


//     - light:
//         type: point
//         position: [0.0, 20.0, -30.0]
//         luminosity: 3.0
//         color: [0.1, 0.1, 0.1]

void AddLightToScene( YAML::Node objNode, Scene *theScene, int debugLevel )
{
  float  x, y, z, r, g, b, lum;
  
  std::string lightType = objNode["type"].as<std::string>();
  if (lightType == "point") {
    YAML::Node pos = objNode["position"];
    x = pos[0].as<float>();
    y = pos[1].as<float>();
    z = pos[2].as<float>();
    if (debugLevel > 0)
      printf("   light with position = %f, %f, %f\n", x, y, z);
    lum = objNode["luminosity"].as<float>();
    if (debugLevel > 0)
      printf("      luminosity = %f\n", lum);
    YAML::Node lightColor = objNode["color"];
    r = lightColor[0].as<float>();
    g = lightColor[1].as<float>();
    b = lightColor[2].as<float>();
    if (debugLevel > 0)
      printf("      light color = %f, %f, %f\n", r,g,b);
    theScene->AddPointLight(Vec3f(x,y,z), Color(r,g,b), lum);
  }
  else
    fprintf(stderr, "ERROR in AddLightToScene: Unrecognized light type (\"%s\")!\n",
    		lightType.c_str());
}


void AddBackgroundToScene( YAML::Node objNode, Scene *theScene, int debugLevel )
{
  float  r, g, b;
  YAML::Node c = objNode["color"];
  r = c[0].as<float>();
  g = c[1].as<float>();
  b = c[2].as<float>();
  theScene->SetBackground(Color(r,g,b));
}


void AddAtmosphereToScene( YAML::Node atmNode, Scene *theScene, int debugLevel )
{
  if (atmNode["IOR"])
    theScene->SetDefaultIOR(atmNode["IOR"].as<float>());
}



/// Allocates and returns a Scene object
Scene* LoadSceneFromFile( std::string sceneFilename, int debugLevel )
{
  int  nObjects;
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
      else if (thisObject["plane"])
        AddPlaneToScene(thisObject["plane"], theScene);
      else if (thisObject["light"])
        AddLightToScene(thisObject["light"], theScene);
      else if (thisObject["background"])
        AddBackgroundToScene(thisObject["background"], theScene);
    }
  }
  
  return theScene;
}
