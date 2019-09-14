// Code for parsing scene files

#include <stdio.h>
#include <string>
#include <vector>
#include "yaml-cpp/yaml.h"
#include "geometry.h"
#include "transform.h"
#include "color.h"
#include "shapes.h"
#include "scene.h"
#include "scenefile_parser.h"


bool VetSceneFile( const std::string &sceneFilename )
{
  return false;
}


float GetFileVersion( const std::string &sceneFilename )
{
  float  versionNum = -1.0;
  YAML::Node sceneFile = YAML::LoadFile(sceneFilename.c_str());
  if (sceneFile["scenefile_version"])
  	versionNum = sceneFile["scenefile_version"].as<float>();
  return versionNum;
}


void AddSphereToScene_new( YAML::Node sphereNode, Scene *theScene, const int debugLevel )
{
  float  x, y, z, radius, reflec, transp;
  float  r, g, b;
  float e_r = 0.f, e_g = 0.f, e_b = 0.f;
  bool  isLight = false;
  Shape *newSphere;
  Transform *transformPtr;

  
  YAML::Node pos = sphereNode["position"];
  x = pos[0].as<float>();
  y = pos[1].as<float>();
  z = pos[2].as<float>();
  if (debugLevel > 0)
    printf("   sphere with position = %f, %f, %f\n", x, y, z);
  Vector deltaPos = Vector(x, y, z);
  transformPtr = new Transform(deltaPos);
  printf("   new sphere with position = %f, %f, %f\n", x, y, z);

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
  
  newSphere = new Sphere(Point(0), radius, Color(r,g,b), reflec, transp, Color(e_r,e_g,e_b));
  theScene->AddShape(newSphere, transformPtr);
}

void AddSphereToScene( YAML::Node sphereNode, Scene *theScene, const int debugLevel )
{
  float  x, y, z, radius, reflec, transp;
  float  r, g, b;
  float e_r = 0.f, e_g = 0.f, e_b = 0.f;
  bool  isLight = false;
  Shape *newSphere;
  Transform *transformPtr = new Transform();  // default Transform (= identity matrix)

  
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
  
  newSphere = new Sphere(Point(x,y,z), radius, Color(r,g,b), reflec, transp, Color(e_r,e_g,e_b));
  theScene->AddShape(newSphere, transformPtr);
}


void AddPlaneToScene( YAML::Node objNode, Scene *theScene, const int debugLevel )
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
    theScene->AddPlane(Point(x,y,z), Vector(n_x,n_y,n_z), Color(r,g,b), reflec, transp,
    						Color(e_r,e_g,e_b));
  else
    theScene->AddPlane(Point(x,y,z), Vector(n_x,n_y,n_z), Color(r,g,b), reflec, transp);
}


//     - light:
//         type: point
//         position: [0.0, 20.0, -30.0]
//         luminosity: 3.0
//         color: [0.1, 0.1, 0.1]

void AddLightToScene( YAML::Node objNode, Scene *theScene, const int debugLevel )
{
  float  x, y, z, r, g, b, lum, radius;

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
      printf("      point light color = %f, %f, %f\n", r,g,b);
    theScene->AddPointLight(Point(x,y,z), Color(r,g,b), lum);
  }
  else if (lightType == "distant") {
    YAML::Node pos = objNode["direction"];
    x = pos[0].as<float>();
    y = pos[1].as<float>();
    z = pos[2].as<float>();
    if (debugLevel > 0)
      printf("   distant light with direction = %f, %f, %f\n", x, y, z);
    lum = objNode["luminosity"].as<float>();
    if (debugLevel > 0)
      printf("      luminosity = %f\n", lum);
    YAML::Node lightColor = objNode["color"];
    r = lightColor[0].as<float>();
    g = lightColor[1].as<float>();
    b = lightColor[2].as<float>();
    if (debugLevel > 0)
      printf("      light color = %f, %f, %f\n", r,g,b);
    theScene->AddDistantLight(Vector(x,y,z), Color(r,g,b), lum);
  }
  else if (lightType == "sphere") {
    YAML::Node pos = objNode["position"];
    x = pos[0].as<float>();
    y = pos[1].as<float>();
    z = pos[2].as<float>();
    if (debugLevel > 0)
      printf("   light with position = %f, %f, %f\n", x, y, z);
    radius = objNode["radius"].as<float>();
    lum = objNode["luminosity"].as<float>();
    if (debugLevel > 0)
      printf("      luminosity = %f\n", lum);
    YAML::Node lightColor = objNode["color"];
    r = lightColor[0].as<float>();
    g = lightColor[1].as<float>();
    b = lightColor[2].as<float>();
    int nsamp = objNode["nsamples"].as<int>();
    if (debugLevel > 0)
      printf("      sphere light color = %f, %f, %f\n", r,g,b);
    theScene->AddSphericalLight(Point(x,y,z), radius, Color(r,g,b), lum, nsamp);
    // add sphere as object if file specified as visible=yes
    if ( (objNode["visible"]) && (objNode["visible"].as<std::string>() == "yes") ) {
      float reflec, transp, e_r, e_g, e_b;
      reflec = transp = 0.0;
      e_r = r;
      e_g = g;
      e_b = b;
      theScene->AddSphere(Point(x,y,z), radius, Color(0), reflec, transp,
    						Color(e_r,e_g,e_b));
    }
  }
  else if (lightType == "rect") {
    YAML::Node pos = objNode["position"];
    x = pos[0].as<float>();
    y = pos[1].as<float>();
    z = pos[2].as<float>();
    if (debugLevel > 0)
      printf("   light with position = %f, %f, %f\n", x, y, z);
    float xSize = objNode["xwidth"].as<float>();
    float zSize = objNode["zwidth"].as<float>();
    lum = objNode["luminosity"].as<float>();
    if (debugLevel > 0)
      printf("      luminosity = %f\n", lum);
    YAML::Node lightColor = objNode["color"];
    r = lightColor[0].as<float>();
    g = lightColor[1].as<float>();
    b = lightColor[2].as<float>();
    int nsamp = objNode["nsamples"].as<int>();
    if (debugLevel > 0)
      printf("      rect light color = %f, %f, %f\n", r,g,b);
    theScene->AddRectLight(Point(x,y,z), xSize, zSize, Color(r,g,b), lum, nsamp);
  }
  else
    fprintf(stderr, "ERROR in AddLightToScene: Unrecognized light type (\"%s\")!\n",
    		lightType.c_str());
}


  //     - material:
  //         type: SimpleMaterial
  //         name: PlainRed
  //         surfaceColor: [0.5, 0.16, 0.18]
  //         reflectionColor: 0
  //         refractionColor: 0
  //         emissionColor: 0
  //         reflection: 0.0
  //         transparency: 0.0
void AddMaterialToScene( YAML::Node objNode, Scene *theScene, const int debugLevel )
{
  float  r, g, b;

  std::string materialType = objNode["type"].as<std::string>();
  std::string materialName = objNode["name"].as<std::string>();

  if (materialType == "SimpleMaterial") {
    YAML::Node surfaceColor = objNode["surfaceColor"];
    r = surfaceColor[0].as<float>();
    g = surfaceColor[1].as<float>();
    b = surfaceColor[2].as<float>();
    Color surfColor = Color(r, g, b);
    if (debugLevel > 0)
      printf("      SimpleMaterial surfaceColor = %f, %f, %f\n", r,g,b);
    YAML::Node reflectionColor = objNode["reflectionColor"];
    r = reflectionColor[0].as<float>();
    g = reflectionColor[1].as<float>();
    b = reflectionColor[2].as<float>();
    Color reflecColor = Color(r, g, b);
    if (debugLevel > 0)
      printf("      SimpleMaterial reflectionColor = %f, %f, %f\n", r,g,b);
    YAML::Node refractionColor = objNode["refractionColor"];
    r = refractionColor[0].as<float>();
    g = refractionColor[1].as<float>();
    b = refractionColor[2].as<float>();
    Color refracColor = Color(r, g, b);
    if (debugLevel > 0)
      printf("      SimpleMaterial refractionColor = %f, %f, %f\n", r,g,b);
    YAML::Node emissionColor = objNode["emissionColor"];
    r = emissionColor[0].as<float>();
    g = emissionColor[1].as<float>();
    b = emissionColor[2].as<float>();
    Color emissColor = Color(r, g, b);
    if (debugLevel > 0)
      printf("      SimpleMaterial emissionColor = %f, %f, %f\n", r,g,b);
    float reflectivity = objNode["reflectivity"].as<float>();
    float transparency = objNode["transparency"].as<float>();
    theScene->AddSimpleMaterial(materialName, surfColor, reflecColor, refracColor,
    							emissColor, reflectivity, transparency);
  }
  else
    fprintf(stderr, "ERROR in AddMaterialToScene: Unrecognized material type (\"%s\")!\n",
    		materialType.c_str());
}


void AddBackgroundToScene( YAML::Node objNode, Scene *theScene, const int debugLevel )
{
  float  r, g, b;
  YAML::Node c = objNode["color"];
  r = c[0].as<float>();
  g = c[1].as<float>();
  b = c[2].as<float>();
  theScene->SetBackground(Color(r,g,b));
}


void AddAtmosphereToScene( YAML::Node atmNode, Scene *theScene, const int debugLevel )
{
  if (atmNode["IOR"])
    theScene->SetDefaultIOR(atmNode["IOR"].as<float>());
}


void AddCameraToScene( YAML::Node objNode, Scene *theScene, const int debugLevel )
{
  float  fieldOfView;
  int  imageWidth, imageHeight;
  fieldOfView = objNode["fov"].as<float>();
  imageWidth = objNode["width"].as<int>();
  imageHeight = objNode["height"].as<int>();
  // for now, we assume the scene has just one camera (instantiated by the
  // scene constructor), and we modify it rather than "adding" it
  Camera *sceneCamera = theScene->GetCamera();
  sceneCamera->SetFOV(fieldOfView);
  sceneCamera->SetImageSize(imageWidth, imageHeight);
}



/// Allocates and returns a Scene object
Scene* LoadSceneFromFile( const std::string &sceneFilename, const int debugLevel )
{
  int  nShapes;
  Scene *theScene = new Scene();
    
  YAML::Node sceneFile = YAML::LoadFile(sceneFilename.c_str());
  
  if (sceneFile["scene"]) {
    nShapes = (int)sceneFile["scene"].size();
    if (debugLevel > 0)
      printf("Scene detected with %d objects.\n", nShapes);
    for (int i = 0; i < nShapes; ++i) {
      YAML::Node thisShape = sceneFile["scene"][i];
      if (thisShape["sphere"])
        AddSphereToScene(thisShape["sphere"], theScene);
      else if (thisShape["sphereT"])
        AddSphereToScene_new(thisShape["sphereT"], theScene);
      else if (thisShape["plane"])
        AddPlaneToScene(thisShape["plane"], theScene);
      else if (thisShape["light"])
        AddLightToScene(thisShape["light"], theScene);
      else if (thisShape["background"])
        AddBackgroundToScene(thisShape["background"], theScene);
    }
  }
  
  return theScene;
}
