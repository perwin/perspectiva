// Code for points, vectors, and rays
// Largely based on Chapter of PBRT, 2nd Ed.

#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <stdio.h>
#include <math.h>



class Vector
{
  public:

    // Constructors with initializations:
    Vector( ) : x(0.0f), y(0.0f), z(0.0f) {}
    Vector( const float aa ) : x(aa), y(aa), z(aa) {}
    Vector( float xx, float yy, float zz ) : x(xx), y(yy), z(zz) {}
    // copy constructor
    Vector ( const Vector &v ) {
      x = v.x;
      y = v.y;
      z = v.z;
    }
    // assignment constructor
    Vector & operator=(const Vector &v) {
      x = v.x;
      y = v.y;
      z = v.z;
      return *this;
    }
    
    // operator overloading to allow vector arithmetic, etc.
    // addition
    Vector operator+(const Vector &v) const {
      return Vector(x + v.x, y + v.y, z + v.z); 
    }
    Vector operator+=(const Vector &v) {
      x += v.x;
      y += v.y;
      z += v.z;
      return *this;
    }

    // subtraction
    Vector operator-(const Vector &v) const {
      return Vector(x - v.x, y - v.y, z - v.z); 
    }
    Vector operator-=(const Vector &v) {
      x -= v.x;
      y -= v.y;
      z -= v.z;
      return *this;
    }

    // scalar multiplication: first definition handles V * s, second handles V *= s
    // (for s * V, see separate function at end of file)
    Vector operator*(float s) const {
      return Vector(s*x, s*y, s*z); 
    }
    Vector & operator*=(float s) {
      x *= s;
      y *= s;
      z *= s;
      return *this;
    }

    Vector operator/(float s) const {
      float inv = 1.0f/s;
      return Vector(inv*x, inv*y, inv*z); 
    }
    Vector & operator/=(float s) {
      float inv = 1.0f/s;
      x *= inv;
      y *= inv;
      z *= inv;
      return *this;
    }

    // negation 
    Vector operator-() const {
      return Vector(-x, -y, -z); 
    }
    
    // equality/inequality
    bool operator==( const Vector &rhs ) const {
      return ((x == rhs.x) && (y == rhs.y) && (z == rhs.z));
    }
    
    bool operator!=( const Vector &rhs ) const { 
      return !(*this == rhs);
    }
    
    // array-style access
    float operator[]( uint8_t i ) const { 
      return (&x)[i]; } 

    // Data members:
    float  x, y, z;
};


class Point
{
  public:
    
    // Constructors with initializations:
    Point( ) : x(0.0f), y(0.0f), z(0.0f) {}
    Point( const float aa ) : x(aa), y(aa), z(aa) {}
    Point( float xx, float yy, float zz ) : x(xx), y(yy), z(zz) {}
    
    // operator overloading to allow vector arithmetic, etc.

    
    // equality/inequality
    bool operator==( const Point &rhs ) const {
      return ((x == rhs.x) && (y == rhs.y) && (z == rhs.z)); }
    bool operator!=( const Point &rhs ) const { 
      return !(*this == rhs); }
    
    // array-style access
    float operator[]( uint8_t i ) const { 
      return (&x)[i]; } 
    
    // generate vector from two points
    Vector operator-( const Point &rhs ) const {
      return Vector(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    // add vector to point to get displaced point
    Point operator+( const Vector &v ) {
      return Point(x + v.x, y + v.y, z + v.z);
    }
    Point operator+=(const Vector &v) {
      x += v.x;
      y += v.y;
      z += v.z;
      return *this;
    }

    // subtract vector from point to get displaced point
    Point operator-( const Vector &v ) {
      return Point(x - v.x, y - v.y, z - v.z);
    }
    Point operator-=(const Vector &v) {
      x -= v.x;
      y -= v.y;
      z -= v.z;
      return *this;
    }
    
    // Data members:
    float  x, y, z;
};



class Ray
{
  public:
    
    Ray( ) { depth = 1; currentIOR = 1.0; }
    // Constructors with initializations:
    Ray( const Point origin, const Vector direction, int dpth=1, float ior=1.0 )
    {
      o = origin;
      dir = direction;
      depth = dpth;
      currentIOR = ior;
    }

    Ray( const Point origin, const Point dest, int dpth=1, float ior=1.0 )
    {
      o = origin;
      dir = dest - origin;
      depth = 1;
      currentIOR = 1.0;
    }

    // calling ray as function: return point at distance t along the ray
    Point operator()(const float t)
    {
      return o + dir*t;
    }

    // Data members:
    Point o;      // origin (point)
    Vector dir;    // direction vector
    int depth;      // current raytrace depth
    float currentIOR;   // index of refraction

};



// Extra function definitions to help with Vector class

// This handles the case of s * V  [which we can't define in the class,
// because member-function definitions include "this" as an extra parameter,
// and definitions using "operator*" only accept two parameters
Vector operator*(float s, const Vector &v) {
  return v*s;
}




#endif  // _GEOMETRY_H_
