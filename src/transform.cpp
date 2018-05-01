// Code for Transform class

#include "transform.h"


  // apply transforms to Points and Vectors
Point Transform::operator()( const Point &p )
{
  float x = p.x, y = p.y, z = p.z;
  float xp = matrix.m[0][0]*x + matrix.m[0][1]*y + matrix.m[0][2]*z + matrix.m[0][3];
  float yp = matrix.m[1][0]*x + matrix.m[1][1]*y + matrix.m[1][2]*z + matrix.m[1][3];
  float zp = matrix.m[2][0]*x + matrix.m[2][1]*y + matrix.m[2][2]*z + matrix.m[2][3];
  float wp = matrix.m[3][0]*x + matrix.m[3][1]*y + matrix.m[3][2]*z + matrix.m[3][3];
  if (wp == 1.0)
    return Point(xp, yp, zp);
  else
    return (Point(xp, yp, zp) / wp);
}

Vector Transform::operator()( const Vector &p )
{
  float x = p.x, y = p.y, z = p.z;
  float xp = matrix.m[0][0]*x + matrix.m[0][1]*y + matrix.m[0][2]*z;
  float yp = matrix.m[1][0]*x + matrix.m[1][1]*y + matrix.m[1][2]*z;
  float zp = matrix.m[2][0]*x + matrix.m[2][1]*y + matrix.m[2][2]*z;
  return Vector(xp, yp, zp);
}
