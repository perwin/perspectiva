#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "vec3.h"


class Sphere 
{ 
public: 
  Vec3f center;                           /// position of the sphere 
  float radius, radius2;                  /// sphere radius and radius^2 
  Vec3f surfaceColor, emissionColor;      /// surface color and emission (light) 
  float transparency, reflection;         /// surface transparency and reflectivity 

  // default constructor
  Sphere( const Vec3f &c, const float &r, const Vec3f &sc, const float &refl = 0, 
    	const float &transp = 0, const Vec3f &ec = 0 ) : 
    center(c), radius(r), radius2(r * r), surfaceColor(sc), emissionColor(ec), 
    transparency(transp), reflection(refl) 
    { /* empty */ }

  bool intersect( const Vec3f &rayorig, const Vec3f &raydir, float &t0, float &t1 ) const;
}; 


#endif  // _GEOMETRY_H_
