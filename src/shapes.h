#ifndef _SHAPES_H_
#define _SHAPES_H_

#include <stdio.h>
#include "vec3.h"
#include "color.h"
#include "materials.h"


class Object
{
public:
  Color surfaceColor, emissionColor;      /// surface color and emission (light) 
  float reflection, transparency;         /// surface transparency and reflectivity 

  // default constructor and destructor
  Object( ) {}; 
  virtual ~Object( ) {}; 

  virtual bool intersect( const Vec3f &rayorig, const Vec3f &raydir, float *t0, float *t1 ) const = 0;

  virtual Vec3f GetNormalAtPoint( const Vec3f &hitPoint ) const = 0;

  virtual void SetMaterial( Material *material )
  {
    objectMaterial = material;
    reflection = objectMaterial->GetReflectivity();
    transparency = objectMaterial->GetTransparency();
    printf("Object: reflection = %.1f, transparency = %.1f\n", reflection, transparency);
    materialPresent = true;
  };
  
  virtual Color GetSurfaceColor( )
  {
    return objectMaterial->GetSurfaceColor();
  };

  virtual Color ComputeObjectColor( Vec3f rayDirection, Vec3f n_hit, Vec3f lightDirection )
  {
    return objectMaterial->ComputeObjectColor(rayDirection, n_hit, lightDirection);
  };
    
  virtual Color GetReflectionColor( )
  {
    return objectMaterial->GetReflectionColor();
  };
    
  virtual Color GetRefractionColor( )
  {
    return objectMaterial->GetRefractionColor();
  };

  virtual Color GetEmissionColor( )
  {
    return objectMaterial->GetEmissionColor();
  };
  
  bool HasSpecular( )
  {
    return objectMaterial->HasSpecular();;
  };
  
protected:
  bool  materialPresent = false;
  Material *objectMaterial;
};


class Sphere : public Object
{ 
public: 
  Vec3f center;                           /// position of the sphere 
  float radius, radius2;                  /// sphere radius and radius^2 

  // default constructor
  Sphere( const Vec3f &cen, float r, const Color &surfColor, float refl = 0, 
    	float transp = 0, const Color &emissColor = 0 )
  {
    center = cen;
    radius = r;
    radius2 = r*r;
    
    objectMaterial = new SimpleMaterial(surfColor, emissColor, Color(0), Color(0), refl,
    									transp);
    materialPresent = true;

    surfaceColor = surfColor;
    emissionColor = emissColor;
    reflection = refl;
    transparency = transp;
  };

  ~Sphere( ) 
  {
    ;
  };
  
  // Defined in shapes.cpp
  bool intersect( const Vec3f &rayorig, const Vec3f &raydir, float *t0, float *t1 ) const;

  Vec3f GetNormalAtPoint( const Vec3f &hitPoint ) const
  {
    return hitPoint - center;
  };


}; 


// FIXME: UNFINISHED! (currently just a renamed copy of Plane)
class Plane : public Object
{ 
public: 
  Vec3f center, norm;

  // default constructor
  Plane( const Vec3f &cen, const Vec3f &n, const Color &surfColor, float refl = 0, 
    	float transp = 0, const Color &emissColor = 0 )
  {
    center = cen;
    norm = n;
    norm.normalize();

    objectMaterial = new SimpleMaterial(surfColor, emissColor, Color(0), Color(0), refl,
    									transp);
    materialPresent = true;

    surfaceColor = surfColor;
    emissionColor = emissColor;
    reflection = refl;
    transparency = transp;
  };

  ~Plane( ) 
  {
    ;
  };
  
  // Defined in shapes.cpp
  bool intersect( const Vec3f &rayorig, const Vec3f &raydir, float *t0, float *t1 ) const;

  Vec3f GetNormalAtPoint( const Vec3f &hitPoint ) const
  {
    return norm;
  };
}; 


class Rectangle : public Object
{ 
public: 
  Vec3f center, norm;

  // default constructor
  Rectangle( const Vec3f &cen, const Vec3f &n, const Color &surfColor, float refl = 0, 
    	float transp = 0, const Color &emissColor = 0 )
  {
    center = cen;
    norm = n;
    norm.normalize();

    objectMaterial = new SimpleMaterial(surfColor, emissColor, Color(0), Color(0), refl,
    									transp);
    materialPresent = true;

    surfaceColor = surfColor;
    emissionColor = emissColor;
    reflection = refl;
    transparency = transp;
  };

  ~Rectangle( ) 
  {
    ;
  };
  
  // Defined in shapes.cpp
  bool intersect( const Vec3f &rayorig, const Vec3f &raydir, float *t0, float *t1 ) const;

  Vec3f GetNormalAtPoint( const Vec3f &hitPoint ) const
  {
    return norm;
  };
}; 


#endif  // _SHAPES_H_
