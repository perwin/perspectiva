#ifndef _SHAPES_H_
#define _SHAPES_H_

#include <stdio.h>
#include "geometry.h"
#include "transform.h"
#include "color.h"
#include "materials.h"


class Shape
{
public:
  Color surfaceColor, emissionColor;      /// surface color and emission (light) 
  float reflection, transparency;         /// surface transparency and reflectivity 

  Transform *ObjectToWorld, *WorldToObject;
  
  // default constructor and destructor
  Shape( ) {}; 
  virtual ~Shape( ) {}; 

  virtual bool intersect( const Point &rayorig, const Vector &raydir, float *t0, float *t1 ) const = 0;

  void AddTransform( Transform *transformPtr )
  {
    ObjectToWorld = transformPtr;
    // FIXME: computer inverse here?
    WorldToObject = transformPtr;
    transformPresent = true;
    
    Point p = (*WorldToObject)(Point(0));
    printf("WorldToObject transforms (0,0,0) to (%.2f,%.2f,%2.f)\n", p.x,p.y,p.z);
  }
  
  virtual Vector GetNormalAtPoint( const Point &hitPoint ) const = 0;

  virtual void SetMaterial( Material *material )
  {
    shapeMaterial = material;
    reflection = shapeMaterial->GetReflectivity();
    transparency = shapeMaterial->GetTransparency();
    printf("Shape: reflection = %.1f, transparency = %.1f\n", reflection, transparency);
    materialPresent = true;
  };
  
  virtual Color GetSurfaceColor( )
  {
    return shapeMaterial->GetSurfaceColor();
  };

  virtual Color ComputeShapeColor( Vector rayDirection, Vector n_hit, Vector lightDirection )
  {
    return shapeMaterial->ComputeShapeColor(rayDirection, n_hit, lightDirection);
  };
    
  virtual Color GetReflectionColor( )
  {
    return shapeMaterial->GetReflectionColor();
  };
    
  virtual Color GetRefractionColor( )
  {
    return shapeMaterial->GetRefractionColor();
  };

  virtual Color GetEmissionColor( )
  {
    return shapeMaterial->GetEmissionColor();
  };
  
  bool HasSpecular( )
  {
    return shapeMaterial->HasSpecular();;
  };
  
protected:
  bool  materialPresent = false;
  bool  transformPresent = false;
  Material *shapeMaterial;
};


class Sphere : public Shape
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
    
    shapeMaterial = new SimpleMaterial(surfColor, emissColor, Color(0), Color(0), refl,
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
    // very simple: normal vector is same direction as radius vector from center
    // of sphere
    return hitPoint - center;
  };


}; 


class Plane : public Shape
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

    shapeMaterial = new SimpleMaterial(surfColor, emissColor, Color(0), Color(0), refl,
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


class Rectangle : public Shape
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

    shapeMaterial = new SimpleMaterial(surfColor, emissColor, Color(0), Color(0), refl,
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
