// Code for points, vectors, and rays
// Largely based on Chapter of PBRT, 2nd Ed.

#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <stdio.h>
#include <cstdlib> 
#include <math.h>
#include "matrix44.h"


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

    // return length^2
    float LengthSquared( ) const {
      return x*x + y*y + z*z;
    }
    // return vector length
    float Length( ) const {
      return sqrtf(LengthSquared());
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
    Point operator+( const Vector &v ) const {
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

    // division by float is useful in transformation matrix math
    Point operator/(float s) const {
      float inv = 1.0f/s;
      return Point(inv*x, inv*y, inv*z); 
    }
    Point & operator/=(float s) {
      float inv = 1.0f/s;
      x *= inv;
      y *= inv;
      z *= inv;
      return *this;
    }

    // Data members:
    float  x, y, z;
};



// A Ray is the combination of a Point (for the origin) and a Vector (for the
// direction from the origin), along with an int for the depth count (current
// raytrace depth) and a float for the current index of refraction.
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
// Declaring these inline seems to help avoid "duplicate symbol" error messages
// during compilation


// This handles the case of s * V  [which we can't define in the class,
// because member-function definitions include "this" as an extra parameter,
// and definitions using "operator*" only accept two parameters
inline Vector operator*(float s, const Vector &v) {
  return v*s;
}
    
// return a normalized copy of the vector
inline Vector Normalize( const Vector &v ) {
  return v / v.Length();
}

inline float Dot( const Vector &v1, const Vector &v2 )
{
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}


#endif  // _GEOMETRY_H_
