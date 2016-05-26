#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>

#include "vec3.h"
#include "color.h"
#include "geometry.h"
#include "lights.h"
#include "cameras.h"


const float DEFAULT_IOR = 1.0;  // for air or vacuum


class Scene
{
public:
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera camera = Camera(30, 640, 480);
  Color  backgroundColor;
  float  defaultIOR;  // default index of refraction for scene
  
  // constructor
  Scene( )
  {
    backgroundColor = Color(1);
    defaultIOR = DEFAULT_IOR;
  }


  void AssembleDefaultScene( )
  {
    Object *objectPtr;
    // position, normal, surface color, reflectivity, transparency
    objectPtr = new Plane(Vec3f( 0.0, -5.5, -15), Vec3f( 0.0, 1.0, 0.0), Color(0.20, 0.20, 0.20), 0, 0.0);
    objects.push_back(objectPtr);
    // position, radius, surface color, reflectivity, transparency, emission color
    objectPtr = new Sphere(Vec3f( 0.0,      0, -20),     2, Color(0.5, 0.16, 0.18), 0, 0.0);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Vec3f( 5.0,     -1, -15),     1, Color(0.90, 0.76, 0.46), 1, 0.0);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Vec3f( 5.0,      0, -25),     1.5, Color(0.65, 0.77, 0.97), 1, 0.5);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Vec3f(-5.5,      0, -15),     1.5, Color(0.90, 0.90, 0.90), 1, 0.0);
    objects.push_back(objectPtr);
    // light: color, luminosity, position
    Light *newLight = new PointLight(Color(1), 1.5e4, Vec3f(0.0, 20.0, -20.0));
    lights.push_back(newLight);
  }


  // For use in regression tests, etc.
  void AssembleTestScene( )
  {
    Object *objectPtr;
    // position, radius, surface color, reflectivity, transparency, emission color
    objectPtr = new Sphere(Vec3f( 0.0, -10004, -20), 10000, Color(0.20, 0.20, 0.20), 0, 0.0);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Vec3f( 0.0,      0, -20),     4, Color(1.0, 0.32, 0.36), 1, 0.5);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Vec3f( 5.0,     -1, -15),     2, Color(0.90, 0.76, 0.46), 1, 0.0);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Vec3f( 5.0,      0, -25),     3, Color(0.65, 0.77, 0.97), 1, 0.0);
    objects.push_back(objectPtr);
    objectPtr = new Sphere(Vec3f(-5.5,      0, -15),     3, Color(0.90, 0.90, 0.90), 1, 0.0);
    objects.push_back(objectPtr);
    // no light, just a luminous sphere
    objectPtr = new Sphere(Vec3f( 0.0,     20, -30),     3, Color(0.00, 0.00, 0.00), 0, 0.0, Color(3));
    objects.push_back(objectPtr);
  }
 
  
  void AddSphere( const Vec3f &pos, const float r, const Color &surfColor, const float reflec, 
  				const float trans, const Color &emissColor=0 )
  {
    Object *objectPtr;
    objectPtr = new Sphere(pos, r, surfColor, reflec, trans, emissColor);
    objects.push_back(objectPtr);
  }


  void AddPlane( const Vec3f &pos, Vec3f norm, const Color &surfColor, const float reflec, 
  				const float trans, const Color &emissColor=0 )
  {
    Object *objectPtr;
    objectPtr = new Plane(pos, norm, surfColor, reflec, trans, emissColor);
    objects.push_back(objectPtr);
  }


  void AddPointLight( const Vec3f &pos, const Color &color, const float luminosity )
  {
    Light *lightPtr = new PointLight(color, luminosity, pos);
    lights.push_back(lightPtr);
  }


  void AddDistantLight( Vec3f dir, const Color &color, const float luminosity )
  {
    Light *lightPtr = new DistantLight(dir, color, luminosity);
    lights.push_back(lightPtr);
  }


  void AddSphericalLight( const Vec3f &pos, const float radius, const Color &color, 
  						const float luminosity, const int nSamples )
  {
    Light *lightPtr = new SphericalLight(pos, radius, color, luminosity, nSamples);
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
};



#endif  // _SCENE_H_
