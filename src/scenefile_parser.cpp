// Code for parsing scene files

#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>  // for std::find

#include "yaml-cpp/yaml.h"
#include "geometry.h"
#include "transform.h"
#include "color.h"
#include "shapes.h"
#include "scene.h"
#include "scenefile_parser.h"

using namespace std;


bool VetSceneFile( const string &sceneFilename )
{
  return false;
}


// If suggested material name (name) == NULL_MATERIAL_NAME, then return false
// (w/o error message). If name is different but is *not* in the vector of valid
// material names, return false *with* an error message. Otherwise, return true.
bool IsValidMaterialName( const string &name, const vector<string> &validMaterialNames,
						int shapeNumber )
{
  if ( find(validMaterialNames.begin(), validMaterialNames.end(), name) 
				!= validMaterialNames.end() )
    return true;
  if (name == NULL_MATERIAL_NAME)
    return false;
  else {
    fprintf(stderr, "** ERROR in LoadSceneFromFile: Shape #%d was assigned", shapeNumber);
	fprintf(stderr, " material named \"%s\", which was not defined in scene file!\n",
	    	name.c_str());
	return false;
  }
}



float GetFileVersion( const string &sceneFilename )
{
  float  versionNum = -1.0;
  YAML::Node sceneFile = YAML::LoadFile(sceneFilename.c_str());
  if (sceneFile["scenefile_version"])
  	versionNum = sceneFile["scenefile_version"].as<float>();
  return versionNum;
}


// void AddSphereToScene_new( YAML::Node sphereNode, Scene *theScene, const int debugLevel )
// {
//   float  x, y, z, radius, reflec, transp;
//   float  r, g, b;
//   float e_r = 0.f, e_g = 0.f, e_b = 0.f;
//   bool  isLight = false;
//   Shape *newSphere;
//   Transform *transformPtr;
// 
//   
//   YAML::Node pos = sphereNode["position"];
//   x = pos[0].as<float>();
//   y = pos[1].as<float>();
//   z = pos[2].as<float>();
//   if (debugLevel > 0)
//     printf("   sphere with position = %f, %f, %f\n", x, y, z);
//   Vector deltaPos = Vector(x, y, z);
//   transformPtr = new Transform(deltaPos);
//   printf("   new sphere with position = %f, %f, %f\n", x, y, z);
// 
//   radius = sphereNode["radius"].as<float>();
//   reflec = sphereNode["reflectivity"].as<float>();
//   transp = sphereNode["transparency"].as<float>();
//   if (debugLevel > 0)
//     printf("      r = %f, reflec = %f, transparency = %f\n", radius, reflec, transp);
//   YAML::Node surfColor = sphereNode["surface_color"];
//   r = surfColor[0].as<float>();
//   g = surfColor[1].as<float>();
//   b = surfColor[2].as<float>();
//   if (debugLevel > 0)
//     printf("      surface_color = %f, %f, %f\n", r,g,b);
//   if (sphereNode["emissivity"]) {
//     YAML::Node emissivity = sphereNode["emissivity"];
//     e_r = emissivity[0].as<float>();
//     e_g = emissivity[1].as<float>();
//     e_b = emissivity[2].as<float>();
//     if (debugLevel > 0)
//       printf("      emissivity = %f, %f, %f\n", e_r,e_g,e_b);
//     if ((e_r > 0.0) || (e_g > 0.0) || (e_b > 0.0))
//       isLight = true;
// 
//   }
//   
//   newSphere = new Sphere(Point(0), radius, Color(r,g,b), reflec, transp, Color(e_r,e_g,e_b));
//   theScene->AddShape(newSphere, transformPtr);
// }

void AddSphereToScene( YAML::Node sphereNode, Scene *theScene, const int debugLevel )
{
  float  x, y, z, radius;
  bool  isLight = false;
  Shape *newSphere;
  Transform *transformPtr = new Transform();  // default Transform (= identity matrix)
  string materialName = NULL_MATERIAL_NAME;
  
  YAML::Node pos = sphereNode["position"];
  x = pos[0].as<float>();
  y = pos[1].as<float>();
  z = pos[2].as<float>();
  if (debugLevel > 0)
    printf("   sphere with position = %f, %f, %f\n", x, y, z);
  radius = sphereNode["radius"].as<float>();
//   reflec = sphereNode["reflectivity"].as<float>();
//   transp = sphereNode["transparency"].as<float>();
//   if (debugLevel > 0)
//     printf("      r = %f, reflec = %f, transparency = %f\n", radius, reflec, transp);
//   YAML::Node surfColor = sphereNode["surface_color"];
//   r = surfColor[0].as<float>();
//   g = surfColor[1].as<float>();
//   b = surfColor[2].as<float>();
//   if (debugLevel > 0)
//     printf("      surface_color = %f, %f, %f\n", r,g,b);
//   if (sphereNode["emissivity"]) {
//     YAML::Node emissivity = sphereNode["emissivity"];
//     e_r = emissivity[0].as<float>();
//     e_g = emissivity[1].as<float>();
//     e_b = emissivity[2].as<float>();
//     if (debugLevel > 0)
//       printf("      emissivity = %f, %f, %f\n", e_r,e_g,e_b);
//     if ((e_r > 0.0) || (e_g > 0.0) || (e_b > 0.0))
//       isLight = true;
//   }
  if (sphereNode["material"]) {
    YAML::Node material = sphereNode["material"];
    materialName = material.as<string>();
    if (debugLevel > 0)
      printf("      material = %s\n", materialName.c_str());
  }
  
  newSphere = new Sphere(Point(x,y,z), radius);
  theScene->AddShape(newSphere, transformPtr, materialName);
}


void AddBoxToScene( YAML::Node boxNode, Scene *theScene, const int debugLevel )
{
  float  x1, y1, z1, x2, y2, z2;
  bool  isLight = false;
  Shape *newBox;
  Transform *transformPtr = new Transform();  // default Transform (= identity matrix)
  string materialName = NULL_MATERIAL_NAME;
  
  YAML::Node pos = boxNode["position"];
  x1 = pos[0].as<float>();
  y1 = pos[1].as<float>();
  z1 = pos[2].as<float>();
  x2 = pos[3].as<float>();
  y2 = pos[4].as<float>();
  z2 = pos[5].as<float>();
  if (debugLevel > 0)
    printf("   box with position = (%f, %f, %f), (%f, %f, %f)\n", 
    		x1, y1, z1, x2, y2, z2);
//   reflec = boxNode["reflectivity"].as<float>();
//   transp = boxNode["transparency"].as<float>();
//   if (debugLevel > 0)
//     printf("      reflec = %f, transparency = %f\n", reflec, transp);
//   YAML::Node surfColor = boxNode["surface_color"];
//   r = surfColor[0].as<float>();
//   g = surfColor[1].as<float>();
//   b = surfColor[2].as<float>();
//   if (debugLevel > 0)
//     printf("      surface_color = %f, %f, %f\n", r,g,b);
//   if (boxNode["emissivity"]) {
//     YAML::Node emissivity = boxNode["emissivity"];
//     e_r = emissivity[0].as<float>();
//     e_g = emissivity[1].as<float>();
//     e_b = emissivity[2].as<float>();
//     if (debugLevel > 0)
//       printf("      emissivity = %f, %f, %f\n", e_r,e_g,e_b);
//     if ((e_r > 0.0) || (e_g > 0.0) || (e_b > 0.0))
//       isLight = true;
//   }
  if (boxNode["material"]) {
    YAML::Node material = boxNode["material"];
    materialName = material.as<string>();
    if (debugLevel > 0)
      printf("      material = %s\n", materialName.c_str());
  }
  
  newBox = new Box(Point(x1,y1,z1), Point(x2,y2,z2));
  theScene->AddShape(newBox, transformPtr, materialName);
}


void AddPlaneToScene( YAML::Node objNode, Scene *theScene, const int debugLevel )
{
  float  x, y, z, n_x, n_y, n_z;
  bool  isLight = false;
  string materialName = NULL_MATERIAL_NAME;
  
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
//   reflec = objNode["reflectivity"].as<float>();
//   transp = objNode["transparency"].as<float>();
//   if (debugLevel > 0)
//     printf("      reflec = %f, transparency = %f\n", reflec, transp);
//   YAML::Node surfColor = objNode["surface_color"];
//   r = surfColor[0].as<float>();
//   g = surfColor[1].as<float>();
//   b = surfColor[2].as<float>();
//   if (debugLevel > 0)
//     printf("      surface_color = %f, %f, %f\n", r,g,b);
//   if (objNode["emissivity"]) {
//     YAML::Node emissivity = objNode["emissivity"];
//     e_r = emissivity[0].as<float>();
//     e_g = emissivity[1].as<float>();
//     e_b = emissivity[2].as<float>();
//     if (debugLevel > 0)
//       printf("      emissivity = %f, %f, %f\n", e_r,e_g,e_b);
//     if ((e_r > 0.0) || (e_g > 0.0) || (e_b > 0.0))
//       isLight = true;
//   }
  if (objNode["material"]) {
    YAML::Node material = objNode["material"];
    materialName = material.as<string>();
    if (debugLevel > 0)
      printf("      material = %s\n", materialName.c_str());
  }

  theScene->AddPlane(Point(x,y,z), Vector(n_x,n_y,n_z));
}


//     - light:
//         type: point
//         position: [0.0, 20.0, -30.0]
//         luminosity: 3.0
//         color: [0.1, 0.1, 0.1]

void AddLightToScene( YAML::Node objNode, Scene *theScene, const int debugLevel )
{
  float  x, y, z, r, g, b, lum, radius;

  string lightType = objNode["type"].as<string>();
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
    if ( (objNode["visible"]) && (objNode["visible"].as<string>() == "yes") ) {
      theScene->AddSphere(Point(x,y,z), radius, NULL_MATERIAL_NAME, Color(r,g,b));
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


void AddMaterialToScene( YAML::Node objNode, Scene *theScene, const int debugLevel )
{
  float  r, g, b;

  string materialType = objNode["type"].as<string>();
  string materialName = objNode["name"].as<string>();

  if (materialType == "Material") {
  
    r = g = b = 1.0;   // Default diffuse color = 1
    if (objNode["baseColor"]) {
      YAML::Node baseCNode = objNode["baseColor"];
      r = baseCNode[0].as<float>();
      g = baseCNode[1].as<float>();
      b = baseCNode[2].as<float>();
    }
    Color baseColor = Color(r, g, b);
    if (debugLevel > 0)
      printf("      Material: %s -- baseColor = %f, %f, %f\n", materialName.c_str(), r,g,b);

    bool metallic = false;
    if (objNode["metal"]) {
      YAML::Node metalNode = objNode["metal"];
      int metal_code = metalNode.as<int>();
      if (metal_code == 1)
        metallic = true;
    }
    if (debugLevel > 0)
      printf("      Material metallic = %d\n", metallic);

    r = g = b = 0.0;   // Default transmission color = 0 [opaque]
    if (objNode["transmissionColor"]) {
      YAML::Node transCNode = objNode["transmissionColor"];
      r = transCNode[0].as<float>();
      g = transCNode[1].as<float>();
      b = transCNode[2].as<float>();
    }
    Color transmissionColor = Color(r, g, b);
    if (debugLevel > 0)
      printf("      Material transmissionColor = %f, %f, %f\n", r,g,b);

    r = g = b = 0.0;   // Default emission color = 0
    if (objNode["emissionColor"]) {
      YAML::Node emissCNode = objNode["emissionColor"];
      r = emissCNode[0].as<float>();
      g = emissCNode[1].as<float>();
      b = emissCNode[2].as<float>();
    }
    Color emissionColor = Color(r, g, b);
    if (debugLevel > 0)
      printf("      Material emissionColor = %f, %f, %f\n", r,g,b);

    float userRoughness = 0.0;
    if (objNode["roughness"])
      userRoughness = objNode["roughness"].as<float>();
    if (debugLevel > 0)
      printf("      Material roughness = %f\n", userRoughness);

    float ior = 1.0;
    if (objNode["ior"])
      ior = objNode["ior"].as<float>();
    if (debugLevel > 0)
      printf("      Material IOR = %f\n", ior);
    theScene->AddMaterial(materialName, baseColor, transmissionColor, emissionColor, metallic,
    						userRoughness, ior);
//     theScene->AddSimpleMaterial(materialName, baseColor, reflecColor, refracColor,
//     							emissColor, reflectivity, transparency, ior);
  }
  else
    fprintf(stderr, "ERROR in AddMaterialToScene: Unrecognized material type (\"%s\")!\n",
    		materialType.c_str());
}


void AddBackgroundToScene( YAML::Node backgroundNode, Scene *theScene, const int debugLevel )
{
  float  r, g, b;
  string  baseName, fileExtension;
  if (backgroundNode["color"]) {
    YAML::Node c = backgroundNode["color"];
    r = c[0].as<float>();
    g = c[1].as<float>();
    b = c[2].as<float>();
    theScene->SetBackground(Color(r,g,b));
  }
  else if (backgroundNode["skybox"]) {
    YAML::Node fname = backgroundNode["skybox"];
    baseName = fname.as<string>();
    YAML::Node extname = backgroundNode["file_extension"];
    fileExtension = extname.as<string>();
    theScene->SetBackgroundSkyBox(baseName, fileExtension);
  }
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
  if (objNode["focal_distance"])
    sceneCamera->SetFocalDistance(objNode["focal_distance"].as<float>());
  if (objNode["aperture_radius"])
    sceneCamera->SetAperture(objNode["aperture_radius"].as<float>());
}



/// Allocates and returns a Scene object
Scene* LoadSceneFromFile( const string &sceneFilename, const int debugLevel )
{
  int  nEntities, nShapes;
  vector<string> validMaterialNames;
  Scene *theScene = new Scene();
    
  YAML::Node sceneFile = YAML::LoadFile(sceneFilename.c_str());
  
  if (sceneFile["scene"]) {
    nEntities = (int)sceneFile["scene"].size();
    if (debugLevel > 0)
      printf("Scene detected with %d entities.\n", nEntities);
    for (int i = 0; i < nEntities; ++i) {
      YAML::Node thisNode = sceneFile["scene"][i];
      // Shapes
      if (thisNode["sphere"])
        AddSphereToScene(thisNode["sphere"], theScene);
//       else if (thisNode["sphereT"])
//         AddSphereToScene_new(thisNode["sphereT"], theScene);
      else if (thisNode["box"])
        AddBoxToScene(thisNode["box"], theScene);
      else if (thisNode["plane"])
        AddPlaneToScene(thisNode["plane"], theScene);
      // Lights
      else if (thisNode["light"])
        AddLightToScene(thisNode["light"], theScene);
      // Materials
      else if (thisNode["material"])
        AddMaterialToScene(thisNode["material"], theScene);
      // Background
      else if (thisNode["background"])
        AddBackgroundToScene(thisNode["background"], theScene);
      // Camera
      else if (thisNode["camera"])
        AddCameraToScene(thisNode["camera"], theScene);
    }

	// Assign materials to Shapes
	nShapes = theScene->shapes.size();
	// Populate vector with keys for Scene.materials map (names of defined materials)
	for ( auto const& keyval: theScene->materials )
	  validMaterialNames.push_back(keyval.first);
	for (int i = 0; i < nShapes; ++i) {
	  string currentMaterialName = theScene->materials_for_shapes[i];
	  if ( IsValidMaterialName(currentMaterialName, validMaterialNames, i) ) {
		// OK, this is a valid material name, so assign the material to this shape
		Material *thisMaterial = theScene->materials[currentMaterialName];
		theScene->shapes[i]->SetMaterial(thisMaterial);
	  } 
	}
  }
  
  return theScene;
}
