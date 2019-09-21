// Color class for raytracing: basically holds r, g, and b floating-point values,
// along with some methods and operator overloading to do arithmetic with color
// values.
//
// This is similar to Vec3f, but labels the internal data members r, g, and b instead
// of x, y, and z; it also leaves out irrelevant operators like // dot and cross 
// products, etc., which make no sense for colors.
// Also, we don't bother making it templated because it will always use float values 
// for data

#ifndef _COLOR_H_
#define _COLOR_H_

#include <cstdlib> 
#include <math.h>


class Color
{
  public:
    
    // Constructors with initializations:
    Color( ) : r(float(0)), g(float(0)), b(float(0)) {}
    Color( const float &aa ) : r(aa), g(aa), b(aa) {}
    Color( float rr, float gg, float bb ) : r(rr), g(gg), b(bb) {}
    
    // operator overloading to allow vector arithmetic, etc.
    Color operator+( const Color &rhs ) const {
      return Color(r + rhs.r, g + rhs.g, b + rhs.b); }
    Color operator-( const Color &rhs ) const {
      return Color(r - rhs.r, g - rhs.g, b - rhs.b); }
    Color& operator+= (const Color &v) { r += v.r, g += v.g, b += v.b; return *this; } 
    // vector multiplication
    Color operator*( const Color &rhs ) const {
      return Color(r * rhs.r, g * rhs.g, b * rhs.b); }
    Color& operator*= (const Color &v) { r *= v.r, g *= v.g, b *= v.b; return *this; } 

    // scalar multiplication ("color * scalar")
    Color operator*( float scalar ) const {
      return Color(scalar*r, scalar*g, scalar*b); }
    // and the reverse order, so we can do "scalar * color" (need to declare as "friend" 
    // so we can use two-argument form and match the "scalar * Color" signature)
    friend Color operator*( float scalar, Color& rhs ) {
      return rhs * scalar; }

    // scalar division (division is not commutative, so no need to create a reverse-
    // order version)
    Color operator/( float scalar ) const {
      return Color(r/scalar, g/scalar, b/scalar); }

    // equality/inequality
    bool operator==( const Color &rhs ) const {
      return ((r == rhs.r) && (g == rhs.g) && (b == rhs.b)); }
    bool operator!=( const Color &rhs ) const { 
      return !(*this == rhs); }

    // access via indexing (I think this assumes that r, g, and b are stored
    // in order next to each other?)
    float operator[]( uint8_t i ) const { 
      return (&r)[i]; } 
    
    
    // Data members:
    float r, g, b;

};


#endif  // _COLOR_H_
