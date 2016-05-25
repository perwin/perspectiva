#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>

#include "vec3.h"
#include "geometry.h"
#include "lights.h"


const float DEFAULT_IOR = 1.0;  // for air or vacuum


class Scene
{
public:
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Vec3f  backgroundColor;
  float  defaultIOR;  // default index of refraction for scene
  
  // constructor
  Scene( )
  {
    backgroundColor = Vec3f(1);
    defaultIOR = DEFAULT_IOR;
  }


  void AssembleDefaultScene( )
  {
    Object *objectPtr;
    // position, radius, surface color, reflectivity, transparency, emission color
    objectPtr = new Plane(Vec3f( 0.0, -5.5, -15), Vec3f( 0.0, 1.0, 0.0), Vec3f(0.20, 0.20, 0.20), 0, 0.0);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Vec3f( 0.0,      0, -20),     2, Vec3f(0.5, 0.16, 0.18), 0, 0.0);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Vec3f( 5.0,     -1, -15),     1, Vec3f(0.90, 0.76, 0.46), 1, 0.0);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Vec3f( 5.0,      0, -25),     1.5, Vec3f(0.65, 0.77, 0.97), 1, 0.5);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Vec3f(-5.5,      0, -15),     1.5, Vec3f(0.90, 0.90, 0.90), 1, 0.0);
    objects.push_back(objectPtr);
    // light
    Light *newLight = new PointLight(Vec3f(1), 1.5e4, Vec3f(0.0, 20.0, -20.0));
    lights.push_back(newLight);
  }


  // For use in regression tests, etc.
  void AssembleTestScene( )
  {
    Object *objectPtr;
    // position, radius, surface color, reflectivity, transparency, emission color
    objectPtr = new Sphere(Vec3f( 0.0, -10004, -20), 10000, Vec3f(0.20, 0.20, 0.20), 0, 0.0);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Vec3f( 0.0,      0, -20),     4, Vec3f(1.0, 0.32, 0.36), 1, 0.5);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Vec3f( 5.0,     -1, -15),     2, Vec3f(0.90, 0.76, 0.46), 1, 0.0);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Vec3f( 5.0,      0, -25),     3, Vec3f(0.65, 0.77, 0.97), 1, 0.0);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Vec3f(-5.5,      0, -15),     3, Vec3f(0.90, 0.90, 0.90), 1, 0.0);
    objects.push_back(objectPtr);
    // no light, just a luminous sphere
    objectPtr = new Sphere(Vec3f( 0.0,     20, -30),     3, Vec3f(0.00, 0.00, 0.00), 0, 0.0, Vec3f(3));
    objects.push_back(objectPtr);
  }
 
  
  void AddSphere( Vec3f pos, float r, Vec3f surfColor, float reflec, 
  				float trans, Vec3f emissColor=0 )
  {
    Object *objectPtr;
    objectPtr = new Sphere(pos, r, surfColor, reflec, trans, emissColor);
    objects.push_back(objectPtr);
  }


  void AddPlane( Vec3f pos, Vec3f norm, Vec3f surfColor, float reflec, 
  				float trans, Vec3f emissColor=0 )
  {
    Object *objectPtr;
    objectPtr = new Plane(pos, norm, surfColor, reflec, trans, emissColor);
    objects.push_back(objectPtr);
  }


  void AddPointLight( Vec3f pos, Vec3f color, float luminosity )
  {
    Light *lightPtr = new PointLight(color, luminosity, pos);
    lights.push_back(lightPtr);
  }


  void SetBackground( Vec3f color )
  {
    backgroundColor = color;
  }

  void SetDefaultIOR( float indexOfRefraction )
  {
    defaultIOR = indexOfRefraction;
  }
};



#endif  // _SCENE_H_
