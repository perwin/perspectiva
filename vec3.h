// Code for raytracing

#ifndef _VEC3_H_
#define _VEC3_H_

#include <cstdlib> 
#include <math.h>

// template class for 3D vector

template <typename T> class Vec3
{
  public:
    
    // Constructors with initializations:
    Vec3( ) : x(T(0)), y(T(0)), z(T(0)) {}
    Vec3( const T &aa ) : x(aa), y(aa), z(aa) {}
    Vec3( T xx, T yy, T zz ) : x(xx), y(yy), z(zz) {}
    
    // operator overloading to allow vector arithmetic, etc.
    Vec3<T> operator+( const Vec3<T> &rhs ) const {
      return Vec3<T>(x + rhs.x, y + rhs.y, z + rhs.z); }
    Vec3<T> operator-( const Vec3<T> &rhs ) const {
      return Vec3<T>(x - rhs.x, y - rhs.y, z - rhs.z); }
    // vector multiplication
    Vec3<T> operator*( const Vec3<T> &rhs ) const {
      return Vec3<T>(x * rhs.x, y * rhs.y, z * rhs.z); }
    Vec3<T>& operator+= (const Vec3<T> &v) { x += v.x, y += v.y, z += v.z; return *this; } 
    Vec3<T>& operator*= (const Vec3<T> &v) { x *= v.x, y *= v.y, z *= v.z; return *this; } 

    // scalar multiplication
    Vec3<T> operator*( T scalar ) const {
      return Vec3<T>(scalar*x, scalar*y, scalar*z); }
    // and the reverse order (need to declare as "friend" so we can use
    // two-argument form and match the "scalar * Vec3<T>" signature)
    friend Vec3<T> operator*( T scalar, Vec3<T>& rhs ) {
      return rhs * scalar; }
    
    T dotProduct( const Vec3<T> &v) const {
      return (x*v.x + y*v.y + z*v.z); }
    T crossProduct( const Vec3<T> &v) const {
      return Vec3<T>(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); }
    T norm( ) const {
      return x*x + y*y + z*z; }
    T length( ) const {
      return sqrt(x*x + y*y + z*z); }
    
    // unary operator
    Vec3<T> operator- ( ) const { return Vec3<T>(-x, -y, -z); }
    
    // access via indexing (I think this assumes that x, y, and z are stored
    // in order next to each other?)
    T operator[]( uint8_t i ) const { 
      return (&x)[i]; } 
    
    // normalize the vector (so it has length = 1), unless it has norm = 0
    // Note that this does *two* things [not sure why]:
    // 1. normalizes the vector;
    // 2. returns a *copy* of the normalized vector
    Vec3<T>& normalize() {
      T  n = norm();
      if (n > 0) {
        T factor = 1.0 / sqrt(n);
        x *= factor;
        y *= factor;
        z *= factor;
      }
      return *this;
    }
    
    // Data members:
    T x, y, z;

};

// Define things outside the class declaration to make it easier to do commutation



// useful shorthands
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;


#endif  // _VEC3_H_
