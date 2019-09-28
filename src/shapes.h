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
//   Color surfaceColor, emissionColor;      /// surface color and emission (light) 
//   float reflection, transparency;         /// surface transparency and reflectivity 

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
    if (materialAllocated)
      delete shapeMaterial;
    shapeMaterial = material;
//     reflection = shapeMaterial->GetReflectivity();
//     transparency = shapeMaterial->GetTransparency();
//     printf("Shape::SetMaterial: reflection = %.1f, transparency = %.1f\n", reflection, transparency);
    materialPresent = true;
  };
  
  virtual Material * GetMaterial( )
  {
    return shapeMaterial;
  }

  
protected:
  bool  materialPresent = false;
  bool  materialAllocated = false;
  bool  transformPresent = false;
  Material *shapeMaterial;
};


class Sphere : public Shape
{ 
public: 
  Point center;                           /// position of the sphere 
  float radius, radius2;                  /// sphere radius and radius^2 

  // default constructor
  Sphere( const Point &cen, float r )
  {
    center = cen;
    radius = r;
    radius2 = r*r;
    
    shapeMaterial = new Material();
    materialPresent = true;
    materialAllocated = true;

//     surfaceColor = surfColor;
//     emissionColor = emissColor;
//     reflection = refl;
//     transparency = transp;
  };

  ~Sphere( ) { ; };
  
  // Defined in shapes.cpp
  bool intersect( const Point &rayorig, const Vector &raydir, float *t0, float *t1 ) const;

  Vector GetNormalAtPoint( const Point &hitPoint ) const
  {
    // very simple: normal vector is same direction as radius vector from center
    // of sphere
    return hitPoint - center;
  };


}; 


// This is a simple AABB (axis-aligned bounding box) rectangular solid
class Box : public Shape
{
public:
  // These specify the lower (minimum values of x,y,z) and upper (maximum
  // values of x,y,z) corners of the box
  Point lowerCorner, upperCorner;
    
  // default constructor
  Box( const Point &minCorner, const Point &maxCorner )
  {
    lowerCorner = minCorner;
    upperCorner = maxCorner;
    
    shapeMaterial = new Material();
    materialPresent = true;
    materialAllocated = true;

//     surfaceColor = surfColor;
//     emissionColor = emissColor;
//     reflection = refl;
//     transparency = transp;
  };
  
  ~Box( ) { ; };

  // Defined in shapes.cpp
  bool intersect( const Point &rayorig, const Vector &raydir, float *t0, float *t1 ) const;

  Vector GetNormalAtPoint( const Point &hitPoint ) const
  {
    Vector deltaLowerCorner = hitPoint - lowerCorner;
    Vector deltaUpperCorner = hitPoint - upperCorner;
    
    if (fabs(deltaLowerCorner.x) < 1.0e-6)
      return Vector(-1.0, 0.0, 0.0);
    if (fabs(deltaUpperCorner.x) < 1.0e-6)
      return Vector(1.0, 0.0, 0.0);
    if (fabs(deltaLowerCorner.y) < 1.0e-6)
      return Vector(0.0, -1.0, 0.0);
    if (fabs(deltaUpperCorner.y) < 1.0e-6)
      return Vector(0.0, 1.0, 0.0);
    if (fabs(deltaLowerCorner.z) < 1.0e-6)
      return Vector(0.0, 0.0, -1.0);
    // If we reach this point, the only possibility left is the near (max-z) face of box
    return Vector(0.0, 0.0, 1.0);
  };

};


class Plane : public Shape
{ 
public: 
  Point center;
  Vector norm;

  // default constructor
  Plane( const Point &cen, const Vector &n )
  {
    center = cen;
    norm = Normalize(n);

    shapeMaterial = new Material();
    materialPresent = true;
    materialAllocated = true;

//     surfaceColor = surfColor;
//     emissionColor = emissColor;
//     reflection = refl;
//     transparency = transp;
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


// FIXME: Currently unfinished! (Needs definition of width/height)
class Rectangle : public Shape
{ 
public: 
  Point center;
  Vector norm;

  // default constructor
  Rectangle( const Point &cen, const Vector &n )
  {
    center = cen;
    norm = Normalize(n);

    shapeMaterial = new Material();
    materialPresent = true;
    materialAllocated = true;

//     surfaceColor = surfColor;
//     emissionColor = emissColor;
//     reflection = refl;
//     transparency = transp;
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
