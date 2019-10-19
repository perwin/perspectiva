#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include <memory>
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

using namespace std;

const float DEFAULT_IOR = 1.0;  // for air or vacuum


class Scene
{
public:
  vector<shared_ptr<Shape>> shapes;
  vector<shared_ptr<Light>> lights;
  vector<string> materials_for_shapes;
  map<string, shared_ptr<Material>> materials;
  shared_ptr<Camera> camera;
  shared_ptr<Environment> environment;
  float  defaultIOR;  // default index of refraction for scene
  Transform *transformPtr;  // replace with vector<Transform *> later...
  
  // constructor
  Scene( )
  {
	environment = make_shared<Environment>(1);
    defaultIOR = DEFAULT_IOR;
    camera = make_shared<Camera>(30, 640, 480);
    transformPtr = new Transform();  // default Transform (= identity matrix)
  }

  // destructor
  ~Scene( )
  {
    delete transformPtr;
  }


  void AssembleDefaultScene( )
  {
    shared_ptr<Shape> shapePtr;
    // position, normal, surface color, reflectivity, transparency
    shapePtr = make_shared<Plane>(Point( 0.0, -5.5, -15), Vector( 0.0, 1.0, 0.0));
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    // position, radius, surface color, reflectivity, transparency, emission color
    shapePtr = make_shared<Sphere>(Point( 0.0,      0, -20),     2);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = make_shared<Sphere>(Point( 5.0,     -1, -15),     1);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = make_shared<Sphere>(Point( 5.0,      0, -25), 1.5);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = make_shared<Sphere>(Point(-5.5,      0, -15),     1.5);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    // light: color, luminosity, position
    shared_ptr<Light> newLight = make_shared<PointLight>(Color(1), 1.5e4, Point(0.0, 20.0, -20.0));
    newLight->AddTransform(transformPtr);
    lights.push_back(newLight);
  }


  // For use in testing new things
  void AssembleTestScene( )
  {
	shared_ptr<Material> plainRed = make_shared<Material>(Color(0.5, 0.16, 0.18), false, false, 0.0);
	shared_ptr<Material> plainGreen = make_shared<Material>(Color(0.18, 0.5, 0.18), false, false, 0.0);
	shared_ptr<Material> goldMirror = make_shared<Material>(Color(0.90, 0.76, 0.46), true, false, 0.0);
	shared_ptr<Material> planeGray = make_shared<Material>(Color(0.20, 0.20, 0.20), false, false, 0.0);
	shared_ptr<Material> almostWhite = make_shared<Material>(Color(0.90, 0.90, 0.90), false, false, 0.0);

    shared_ptr<Shape> shapePtr;
    // position, normal, surface color, reflectivity, transparency
    shapePtr = make_shared<Plane>(Point( 0.0, -5.5, -15), Vector( 0.0, 1.0, 0.0));
    shapePtr->SetMaterial(planeGray);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);

    // position, radius, surface color, reflectivity, transparency, emission color
    // plain red sphere
    shapePtr = make_shared<Sphere>(Point( -5.0,      0, -25),     2);
    shapePtr->SetMaterial(plainRed);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    // gold-ish mirror sphere
    shapePtr = make_shared<Sphere>(Point( 0.0,     0, -25),     2);
    shapePtr->SetMaterial(goldMirror);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);

    // transparent sphere
    shapePtr = make_shared<Sphere>(Point( 5.0,     0, -25),     2);
    shapePtr->SetMaterial(almostWhite);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    
    // "background shapes" = row of small green spheres
    // plain green sphere
    shapePtr = make_shared<Sphere>(Point( -12.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = make_shared<Sphere>(Point( -9.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = make_shared<Sphere>(Point( -6.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = make_shared<Sphere>(Point( -3.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = make_shared<Sphere>(Point( 0.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = make_shared<Sphere>(Point( 3.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = make_shared<Sphere>(Point( 6.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = make_shared<Sphere>(Point( 9.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    shapePtr = make_shared<Sphere>(Point( 12.0,      -1, -50),     1);
    shapePtr->SetMaterial(plainGreen);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    
    // light: color, luminosity, position
    shared_ptr<Light> newLight = make_shared<PointLight>(Color(1), 1.5e4, Point(0.0, 20.0, -20.0));
    newLight->AddTransform(transformPtr);
    lights.push_back(newLight);
  }
 
  
  void AddShape( shared_ptr<Shape> shapePtr, Transform *shapeTransform, 
  				const string materialName=NULL_MATERIAL_NAME )
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
  				const string materialName=NULL_MATERIAL_NAME,
  				Color emissionColor=Color(0) )
  {
    shared_ptr<Shape> shapePtr;
    shapePtr = make_shared<Sphere>(pos, r);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    materials_for_shapes.push_back(materialName);
    // the following is for the case where we specified a spherical light and
    // want it to be visible
    if ( (emissionColor.r > 0) || (emissionColor.g > 0) || (emissionColor.b > 0) ) {
      shared_ptr<Material> emissMaterial = make_shared<Material>(Color(0), false, false, 0.0, Color(0), 1.0,
      										emissionColor);
      shapePtr->SetMaterial(emissMaterial);
    }
  }

  void AddPlane( const Point &pos, Vector norm, 
  				const string materialName=NULL_MATERIAL_NAME )
  {
    shared_ptr<Shape> shapePtr;
    shapePtr = make_shared<Plane>(pos, norm);
    shapePtr->AddTransform(transformPtr);
    shapes.push_back(shapePtr);
    materials_for_shapes.push_back(materialName);
  }


  void AddPointLight( const Point &pos, const Color &color, const float luminosity )
  {
    shared_ptr<Light> lightPtr = make_shared<PointLight>(color, luminosity, pos);
    lightPtr->AddTransform(transformPtr);
    lights.push_back(lightPtr);
  }


  void AddDistantLight( Vector dir, const Color &color, const float luminosity )
  {
    shared_ptr<Light> lightPtr = make_shared<DistantLight>(dir, color, luminosity);
    lightPtr->AddTransform(transformPtr);
    lights.push_back(lightPtr);
  }


  void AddSphericalLight( const Point &pos, const float radius, const Color &color, 
  						const float luminosity, const int nSamples )
  {
    shared_ptr<Light> lightPtr = make_shared<SphericalLight>(pos, radius, color, luminosity, nSamples);
    lightPtr->AddTransform(transformPtr);
    lights.push_back(lightPtr);
  }


  void AddRectLight( const Point &pos, const float xSize, const float zSize, 
  						const Color &color, const float luminosity, const int nSamples )
  {
    shared_ptr<Light> lightPtr = make_shared<RectLight>(pos, xSize, zSize, color, luminosity, nSamples);
    lightPtr->AddTransform(transformPtr);
    lights.push_back(lightPtr);
  }

  void AddMaterial( string name, const Color &baseColor, const Color transmitColor,
  					const Color &emissColor, bool metallic, bool specular, 
  					float userRoughness, float ior)
  {
    shared_ptr<Material> materialPtr = make_shared<Material>(baseColor, metallic, specular, userRoughness,
    									transmitColor, ior, emissColor);
    materials[name] = materialPtr;
  }


  void SetBackground( const Color &color )
  {
    environment->SetEnvironmentColor(color);
  }


  void SetBackgroundSphereMap( const string fileName )
  {
    environment->AddSphereMap(fileName);
  }


  void SetBackgroundSkyBox( const string baseFilename, const string fileExtension )
  {
    environment->AddSkyBox(baseFilename, fileExtension);
  }


  void SetDefaultIOR( const float indexOfRefraction )
  {
    defaultIOR = indexOfRefraction;
  }


  shared_ptr<Camera> GetCamera( )
  {
    return camera;
  }

};



#endif  // _SCENE_H_
