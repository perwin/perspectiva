#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include <stdio.h>

#include "definitions.h"
#include "geometry.h"
#include "transform.h"
#include "color.h"
#include "shapes.h"
#include "lights.h"
#include "materials.h"
#include "cameras.h"
#include "environment_map.h"


const float DEFAULT_IOR = 1.0;  // for air or vacuum


class Scene
{
public:
  std::vector<Shape *> shapes;
  std::vector<Light *> lights;
  std::vector<std::string> materials_for_shapes;
  std::map<std::string, Material *> materials;
  Camera * camera;
  Environment * environment;
//   Color  backgroundColor;
  float  defaultIOR;  // default index of refraction for scene
  Transform *transformPtr;  // replace with vector<Transform *> later...
  
  // constructor
  Scene( )
  {
//     backgroundColor = Color(1);
	environment = new Environment(1);
    defaultIOR = DEFAULT_IOR;
    camera = new Camera(30, 640, 480);
    transformPtr = new Transform();  // default Transform (= identity matrix)
  }

  // destructor
  ~Scene( )
  {
    delete camera;
    delete transformPtr;
  }


  void AssembleDefaultScene( )
  {
    Shape *shapePtr;
    // position, normal, surface color, reflectivity, transparency
//     shapePtr = new Plane(Point( 0.0, -5.5, -15), Vector( 0.0, 1.0, 0.0), Color(0.20, 0.20, 0.20), 0, 0.0);
    shapePtr = new Plane(Point( 0.0, -5.5, -15), Vector( 0.0, 1.0, 0.0));
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    // position, radius, surface color, reflectivity, transparency, emission color
//     shapePtr = new Sphere(Point( 0.0,      0, -20),     2, Color(0.5, 0.16, 0.18), 0, 0.0);
    shapePtr = new Sphere(Point( 0.0,      0, -20),     2);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
//     shapePtr = new Sphere(Point( 5.0,     -1, -15),     1, Color(0.90, 0.76, 0.46), 1, 0.0);
    shapePtr = new Sphere(Point( 5.0,     -1, -15),     1);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
//     shapePtr = new Sphere(Point( 5.0,      0, -25),     1.5, Color(0.65, 0.77, 0.97), 1, 0.5);
    shapePtr = new Sphere(Point( 5.0,      0, -25), 1.5);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
//     shapePtr = new Sphere(Point(-5.5,      0, -15),     1.5, Color(0.90, 0.90, 0.90), 1, 0.0);
    shapePtr = new Sphere(Point(-5.5,      0, -15),     1.5);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    // light: color, luminosity, position
    Light *newLight = new PointLight(Color(1), 1.5e4, Point(0.0, 20.0, -20.0));
    newLight->AddTransform(transformPtr);
    lights.push_back(newLight);
  }


  // For use in testing new things
  void AssembleTestScene( )
  {
//     Material( Color baseC, bool metal, bool specular, float userRoughness, Color transmissionC=Color(0), 
//     		float ior=1.5, Color emissC=Color(0) )
	Material *plainRed = new Material(Color(0.5, 0.16, 0.18), false, false, 0.0);
	Material *plainGreen = new Material(Color(0.18, 0.5, 0.18), false, false, 0.0);
	Material *goldMirror = new Material(Color(0.90, 0.76, 0.46), true, false, 0.0);
	Material *planeGray = new Material(Color(0.20, 0.20, 0.20), false, false, 0.0);
	Material *almostWhite = new Material(Color(0.90, 0.90, 0.90), false, false, 0.0);

    Shape *shapePtr;
    // position, normal, surface color, reflectivity, transparency
    shapePtr = new Plane(Point( 0.0, -5.5, -15), Vector( 0.0, 1.0, 0.0));
    shapePtr->SetMaterial(planeGray);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);

    // position, radius, surface color, reflectivity, transparency, emission color
    // plain red sphere
    shapePtr = new Sphere(Point( -5.0,      0, -25),     2);
    shapePtr->SetMaterial(plainRed);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    // gold-ish mirror sphere
    shapePtr = new Sphere(Point( 0.0,     0, -25),     2);
    shapePtr->SetMaterial(goldMirror);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);

    // transparent sphere
    shapePtr = new Sphere(Point( 5.0,     0, -25),     2);
    shapePtr->SetMaterial(almostWhite);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    
    // "background shapes" = row of small green spheres
    // plain green sphere
    shapePtr = new Sphere(Point( -12.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = new Sphere(Point( -9.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = new Sphere(Point( -6.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = new Sphere(Point( -3.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = new Sphere(Point( 0.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = new Sphere(Point( 3.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = new Sphere(Point( 6.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = new Sphere(Point( 9.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = new Sphere(Point( 12.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    
    // light: color, luminosity, position
    Light *newLight = new PointLight(Color(1), 1.5e4, Point(0.0, 20.0, -20.0));
    newLight->AddTransform(transformPtr);
    lights.push_back(newLight);
  }
  
  // This was a default test scene
//   void AssembleTestScene( )
//   {
//     Shape *shapePtr;
//     // position, radius, surface color, reflectivity, transparency, emission color
//     shapePtr = new Sphere(Point( 0.0, -10004, -20), 10000, Color(0.20, 0.20, 0.20), 0, 0.0);
//     shapes.push_back(shapePtr);
//     shapePtr = new Sphere(Point( 0.0,      0, -20),     4, Color(1.0, 0.32, 0.36), 1, 0.5);
//     shapes.push_back(shapePtr);
//     shapePtr = new Sphere(Point( 5.0,     -1, -15),     2, Color(0.90, 0.76, 0.46), 1, 0.0);
//     shapes.push_back(shapePtr);
//     shapePtr = new Sphere(Point( 5.0,      0, -25),     3, Color(0.65, 0.77, 0.97), 1, 0.0);
//     shapes.push_back(shapePtr);
//     shapePtr = new Sphere(Point(-5.5,      0, -15),     3, Color(0.90, 0.90, 0.90), 1, 0.0);
//     shapes.push_back(shapePtr);
//     // no light, just a luminous sphere
//     shapePtr = new Sphere(Point( 0.0,     20, -30),     3, Color(0.00, 0.00, 0.00), 0, 0.0, Color(3));
//     shapes.push_back(shapePtr);
//   }
 
  
  void AddShape( Shape *shapePtr, Transform *shapeTransform, 
  				const std::string materialName=NULL_MATERIAL_NAME )
  {
    // FIXME (later): check to see if shape's transform is already in list/vector of
    // Transform shapes; if yes, use pointer to existing instance and delete the
    // one that was passed in
    shapePtr->AddTransform(shapeTransform);

    shapes.push_back(shapePtr);
    materials_for_shapes.push_back(materialName);
    
    // FIXME: check to see if shape emits; add to lights vector?
  }

  void AddSphere( const Point &pos, const float r, 
  				const std::string materialName=NULL_MATERIAL_NAME,
  				Color emissionColor=Color(0) )
  {
    Shape *shapePtr;
    shapePtr = new Sphere(pos, r);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    materials_for_shapes.push_back(materialName);
    // the following is for the case where we specified a spherical light and
    // want it to be visible
    if ( (emissionColor.r > 0) || (emissionColor.g > 0) || (emissionColor.b > 0) ) {
      Material *emissMaterial = new Material(Color(0), false, false, 0.0, Color(0), 1.0,
      										emissionColor);
      shapePtr->SetMaterial(emissMaterial);
    }
  }

  void AddPlane( const Point &pos, Vector norm, 
  				const std::string materialName=NULL_MATERIAL_NAME )
  {
    Shape *shapePtr;
    shapePtr = new Plane(pos, norm);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    materials_for_shapes.push_back(materialName);
  }


  void AddPointLight( const Point &pos, const Color &color, const float luminosity )
  {
    Light *lightPtr = new PointLight(color, luminosity, pos);
    lightPtr->AddTransform(transformPtr);
    lights.push_back(lightPtr);
  }


  void AddDistantLight( Vector dir, const Color &color, const float luminosity )
  {
    Light *lightPtr = new DistantLight(dir, color, luminosity);
    lightPtr->AddTransform(transformPtr);
    lights.push_back(lightPtr);
  }


  void AddSphericalLight( const Point &pos, const float radius, const Color &color, 
  						const float luminosity, const int nSamples )
  {
    Light *lightPtr = new SphericalLight(pos, radius, color, luminosity, nSamples);
    lightPtr->AddTransform(transformPtr);
    lights.push_back(lightPtr);
  }


  void AddRectLight( const Point &pos, const float xSize, const float zSize, 
  						const Color &color, const float luminosity, const int nSamples )
  {
    Light *lightPtr = new RectLight(pos, xSize, zSize, color, luminosity, nSamples);
    lightPtr->AddTransform(transformPtr);
    lights.push_back(lightPtr);
  }

  void AddMaterial( string name, const Color &baseColor, const Color transmitColor,
  					const Color &emissColor, bool metallic, bool specular, 
  					float userRoughness, float ior)
  {
    Material *materialPtr = new Material(baseColor, metallic, specular, userRoughness,
    									transmitColor, ior, emissColor);
    materials[name] = materialPtr;
  }


  void SetBackground( const Color &color )
  {
    environment->SetEnvironmentColor(color);
  }


  void SetBackgroundSkyBox( const string baseFilename, const string fileExtension )
  {
    environment->AddSkyBox(baseFilename, fileExtension);
  }


  void SetDefaultIOR( const float indexOfRefraction )
  {
    defaultIOR = indexOfRefraction;
  }


  Camera * GetCamera( )
  {
    return camera;
  }

};



#endif  // _SCENE_H_
