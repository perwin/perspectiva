#ifndef _SHAPES_H_
#define _SHAPES_H_

#include <stdio.h>
#include "geometry.h"
#include "transform.h"
#include "color.h"
#include "materials.h"


class Object
{
public:
  Color surfaceColor, emissionColor;      /// surface color and emission (light) 
  float reflection, transparency;         /// surface transparency and reflectivity 

  Transform *ObjectToWorld, *WorldToObject;
  
  // default constructor and destructor
  Object( ) {}; 
  virtual ~Object( ) {}; 

  virtual bool intersect( const Point &rayorig, const Vector &raydir, float *t0, float *t1 ) const = 0;

  void AddTransform( Transform *transformPtr )
  {
    ObjectToWorld = transformPtr;
    // FIXME: computer inverse here?
    WorldToObject = transformPtr;
    transformPresent = true;
  }
  
  virtual Vector GetNormalAtPoint( const Point &hitPoint ) const = 0;

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

  virtual Color ComputeObjectColor( Vector rayDirection, Vector n_hit, Vector lightDirection )
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
  bool  transformPresent = false;
  Material *objectMaterial;
};


class Sphere : public Object
{ 
public: 
  Point center;                           /// position of the sphere 
  float radius, radius2;                  /// sphere radius and radius^2 

  // default constructor
  Sphere( const Point &cen, float r, const Color &surfColor, float refl = 0, 
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
  bool intersect( const Point &rayorig, const Vector &raydir, float *t0, float *t1 ) const;

  Vector GetNormalAtPoint( const Point &hitPoint ) const
  {
    return hitPoint - center;
  };


}; 


// FIXME: UNFINISHED! (currently just a renamed copy of Plane)
class Plane : public Object
{ 
public: 
  Point center;
  Vector norm;

  // default constructor
  Plane( const Point &cen, const Vector &n, const Color &surfColor, float refl = 0, 
    	float transp = 0, const Color &emissColor = 0 )
  {
    center = cen;
    norm = Normalize(n);

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
  bool intersect( const Point &rayorig, const Vector &raydir, float *t0, float *t1 ) const;

  Vector GetNormalAtPoint( const Point &hitPoint ) const
  {
    return norm;
  };
}; 


class Rectangle : public Object
{ 
public: 
  Point center;
  Vector norm;

  // default constructor
  Rectangle( const Point &cen, const Vector &n, const Color &surfColor, float refl = 0, 
    	float transp = 0, const Color &emissColor = 0 )
  {
    center = cen;
    norm = Normalize(n);

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
  bool intersect( const Point &rayorig, const Vector &raydir, float *t0, float *t1 ) const;

  Vector GetNormalAtPoint( const Point &hitPoint ) const
  {
    return norm;
  };
}; 


#endif  // _SHAPES_H_
