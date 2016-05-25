#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "vec3.h"


class Object
{
public:
  Vec3f surfaceColor, emissionColor;      /// surface color and emission (light) 
  float reflection, transparency;         /// surface transparency and reflectivity 

  // default constructor and destructor
  Object( ) {}; 
  virtual ~Object( ) {;}; 

  virtual bool intersect( const Vec3f &rayorig, const Vec3f &raydir, float *t0, float *t1 ) const = 0;

  virtual Vec3f GetNormalAtPoint( const Vec3f &hitPoint ) const = 0;
};


class Sphere : public Object
{ 
public: 
  Vec3f center;                           /// position of the sphere 
  float radius, radius2;                  /// sphere radius and radius^2 

  // default constructor
  Sphere( const Vec3f &cen, const float &r, const Vec3f &surfColor, const float &refl = 0, 
    	const float &transp = 0, const Vec3f &emissColor = 0 )
  {
    center = cen;
    radius = r;
    radius2 = r*r;
    surfaceColor = surfColor;
    emissionColor = emissColor;
    reflection = refl;
    transparency = transp;
  };

  ~Sphere( ) 
  {
    ;
  };
  
  // Defined in geometry.cpp
  bool intersect( const Vec3f &rayorig, const Vec3f &raydir, float *t0, float *t1 ) const;

  Vec3f GetNormalAtPoint( const Vec3f &hitPoint ) const
  {
    return hitPoint - center;
  };


}; 


class Plane : public Object
{ 
public: 
  Vec3f center, norm;

  // default constructor
  Plane( const Vec3f &cen, const Vec3f &n, const Vec3f &surfColor, const float &refl = 0, 
    	const float &transp = 0, const Vec3f &emissColor = 0 )
  {
    center = cen;
    norm = n;
    norm.normalize();
    surfaceColor = surfColor;
    emissionColor = emissColor;
    reflection = refl;
    transparency = transp;
  };

  ~Plane( ) 
  {
    ;
  };
  
  // Defined in geometry.cpp
  bool intersect( const Vec3f &rayorig, const Vec3f &raydir, float *t0, float *t1 ) const;

  Vec3f GetNormalAtPoint( const Vec3f &hitPoint ) const
  {
    return norm;
  };
}; 


#endif  // _GEOMETRY_H_
