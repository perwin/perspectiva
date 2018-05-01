#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include <stdio.h>

#include "geometry.h"
#include "transform.h"
#include "color.h"
#include "shapes.h"
#include "lights.h"
#include "cameras.h"


const float DEFAULT_IOR = 1.0;  // for air or vacuum


class Scene
{
public:
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera * camera;
  Color  backgroundColor;
  float  defaultIOR;  // default index of refraction for scene
  Transform *transformPtr;  // replace with vector<Transform *> later...
  
  // constructor
  Scene( )
  {
    backgroundColor = Color(1);
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
    Object *objectPtr;
    // position, normal, surface color, reflectivity, transparency
    objectPtr = new Plane(Point( 0.0, -5.5, -15), Vector( 0.0, 1.0, 0.0), Color(0.20, 0.20, 0.20), 0, 0.0);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
    // position, radius, surface color, reflectivity, transparency, emission color
    objectPtr = new Sphere(Point( 0.0,      0, -20),     2, Color(0.5, 0.16, 0.18), 0, 0.0);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Point( 5.0,     -1, -15),     1, Color(0.90, 0.76, 0.46), 1, 0.0);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Point( 5.0,      0, -25),     1.5, Color(0.65, 0.77, 0.97), 1, 0.5);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Point(-5.5,      0, -15),     1.5, Color(0.90, 0.90, 0.90), 1, 0.0);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
    // light: color, luminosity, position
    Light *newLight = new PointLight(Color(1), 1.5e4, Point(0.0, 20.0, -20.0));
    newLight->AddTransform(transformPtr);
    lights.push_back(newLight);
  }


  // For use in testing new things
  void AssembleTestScene( )
  {
    Object *objectPtr;
    // position, normal, surface color, reflectivity, transparency
    objectPtr = new Plane(Point( 0.0, -5.5, -15), Vector( 0.0, 1.0, 0.0), Color(0.20, 0.20, 0.20), 0, 0.0);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);

	Material *plainRed = new SimpleMaterial(Color(0.5, 0.16, 0.18), Color(0), 
									Color(0), Color(0), 0.0, 0.0);
//     printf("plainRed: reflectivity = %.1f, transparency = %.1f\n", plainRed->GetReflectivity(),
//     		plainRed->GetTransparency());
	Material *goldMirror = new SimpleMaterial(Color(0.90, 0.76, 0.46), Color(0), 
									Color(0), Color(0), 1.0, 0.0);
    // position, radius, surface color, reflectivity, transparency, emission color
    // plain red sphere
//    objectPtr = new Sphere(Point( -5.0,      0, -25),     2, Color(0.5, 0.16, 0.18), 0, 0.0);
    objectPtr = new Sphere(Point( -5.0,      0, -25),     2, Color(0), 0, 0.0);
    objectPtr->SetMaterial(plainRed);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
    // gold-ish mirror sphere
//     objectPtr = new Sphere(Point( 0.0,     0, -25),     2, Color(0.90, 0.76, 0.46), 1, 0.0);
    objectPtr = new Sphere(Point( 0.0,     0, -25),     2, Color(0), 0.0, 0.0);
    objectPtr->SetMaterial(goldMirror);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);

    // transparent sphere
    objectPtr = new Sphere(Point( 5.0,     0, -25),     2, Color(0.9, 0.9, 0.9), 0.0, 0.9);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
    
    // "background objects" = row of small green spheres
    // plain green sphere
    objectPtr = new Sphere(Point( -12.0,      -1, -50),     1, Color(0.18, 0.5, 0.18), 0, 0.0);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Point( -9.0,      -1, -50),     1, Color(0.18, 0.5, 0.18), 0, 0.0);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Point( -6.0,      -1, -50),     1, Color(0.18, 0.5, 0.18), 0, 0.0);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Point( -3.0,      -1, -50),     1, Color(0.18, 0.5, 0.18), 0, 0.0);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Point( 0.0,      -1, -50),     1, Color(0.18, 0.5, 0.18), 0, 0.0);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Point( 3.0,      -1, -50),     1, Color(0.18, 0.5, 0.18), 0, 0.0);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Point( 6.0,      -1, -50),     1, Color(0.18, 0.5, 0.18), 0, 0.0);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Point( 9.0,      -1, -50),     1, Color(0.18, 0.5, 0.18), 0, 0.0);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Point( 12.0,      -1, -50),     1, Color(0.18, 0.5, 0.18), 0, 0.0);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
    
    // light: color, luminosity, position
    Light *newLight = new PointLight(Color(1), 1.5e4, Point(0.0, 20.0, -20.0));
    newLight->AddTransform(transformPtr);
    lights.push_back(newLight);
  }
  
  // This was a default test scene
//   void AssembleTestScene( )
//   {
//     Object *objectPtr;
//     // position, radius, surface color, reflectivity, transparency, emission color
//     objectPtr = new Sphere(Point( 0.0, -10004, -20), 10000, Color(0.20, 0.20, 0.20), 0, 0.0);
//     objects.push_back(objectPtr);
//     objectPtr = new Sphere(Point( 0.0,      0, -20),     4, Color(1.0, 0.32, 0.36), 1, 0.5);
//     objects.push_back(objectPtr);
//     objectPtr = new Sphere(Point( 5.0,     -1, -15),     2, Color(0.90, 0.76, 0.46), 1, 0.0);
//     objects.push_back(objectPtr);
//     objectPtr = new Sphere(Point( 5.0,      0, -25),     3, Color(0.65, 0.77, 0.97), 1, 0.0);
//     objects.push_back(objectPtr);
//     objectPtr = new Sphere(Point(-5.5,      0, -15),     3, Color(0.90, 0.90, 0.90), 1, 0.0);
//     objects.push_back(objectPtr);
//     // no light, just a luminous sphere
//     objectPtr = new Sphere(Point( 0.0,     20, -30),     3, Color(0.00, 0.00, 0.00), 0, 0.0, Color(3));
//     objects.push_back(objectPtr);
//   }
 
  
  void AddSphere( const Point &pos, const float r, const Color &surfColor, const float reflec, 
  				const float trans, const Color &emissColor=0 )
  {
    Object *objectPtr;
    objectPtr = new Sphere(pos, r, surfColor, reflec, trans, emissColor);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
  }


  void AddPlane( const Point &pos, Vector norm, const Color &surfColor, const float reflec, 
  				const float trans, const Color &emissColor=0 )
  {
    Object *objectPtr;
    objectPtr = new Plane(pos, norm, surfColor, reflec, trans, emissColor);
    objectPtr->AddTransform(transformPtr);
    objects.push_back(objectPtr);
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


  void SetBackground( const Color &color )
  {
    backgroundColor = color;
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
