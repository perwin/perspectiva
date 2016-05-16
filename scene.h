#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>

#include "vec3.h"
#include "geometry.h"


class Scene
{
public:
  std::vector<Sphere> objects;
  
  // constructor
  Scene( ) { /* empty */ }

  void AssembleDefaultScene( )
  {
    // position, radius, surface color, reflectivity, transparency, emission color
    objects.push_back(Sphere(Vec3f( 0.0, -10004, -20), 10000, Vec3f(0.20, 0.20, 0.20), 0, 0.0));
    objects.push_back(Sphere(Vec3f( 0.0,      0, -20),     4, Vec3f(1.00, 0.32, 0.36), 1, 0.5));
    objects.push_back(Sphere(Vec3f( 5.0,     -1, -15),     2, Vec3f(0.90, 0.76, 0.46), 1, 0.0));
    objects.push_back(Sphere(Vec3f( 5.0,      0, -25),     3, Vec3f(0.65, 0.77, 0.97), 1, 0.0));
    objects.push_back(Sphere(Vec3f(-5.5,      0, -15),     3, Vec3f(0.90, 0.90, 0.90), 1, 0.0));
    // light
    objects.push_back(Sphere(Vec3f( 0.0,     20, -30),     3, Vec3f(0.00, 0.00, 0.00), 0, 0.0, Vec3f(3)));
  }
  
  void AddSphere( Vec3f pos, float r, Vec3f surfColor, float reflec, 
  				float trans, Vec3f emissColor=0 )
  {
    objects.push_back(Sphere(pos, r, surfColor, reflec, trans, emissColor));
  }
};



#endif  // _SCENE_H_
