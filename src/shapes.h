#ifndef _SHAPES_H_
#define _SHAPES_H_

#include <stdio.h>
#include "geometry.h"
#include "transform.h"
#include "color.h"
#include "materials.h"

using namespace std;


class Shape
{
public:

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
//     printf("WorldToObject transforms (0,0,0) to (%.2f,%.2f,%2.f)\n", p.x,p.y,p.z);
  }
  
  virtual Vector GetNormalAtPoint( const Point &hitPoint ) const = 0;

  virtual void SetMaterial( shared_ptr<Material> material )
  {
    shapeMaterial = material;
    materialPresent = true;
  };
  
  virtual shared_ptr<Material> GetMaterial( )
  {
    return shapeMaterial;
  }

  
protected:
  bool  materialPresent = false;
//   bool  materialAllocated = false;
  bool  transformPresent = false;
  shared_ptr<Material> shapeMaterial;
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
    
    shapeMaterial = make_shared<Material>();
    materialPresent = true;
//     materialAllocated = true;
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
    
    shapeMaterial = make_shared<Material>();
    materialPresent = true;
  };
  
  ~Box( ) { ; };

  // Defined in shapes.cpp
  bool intersect( const Point &rayorig, const Vector &raydir, float *t0, float *t1 ) const;

  Vector GetNormalAtPoint( const Point &hitPoint ) const;

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

    shapeMaterial = make_shared<Material>();
    materialPresent = true;
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

    shapeMaterial = make_shared<Material>();
    materialPresent = true;
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
