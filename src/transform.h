// Code for 4x4 matrices using homogeneous coordinates for transforming 
// Points, Vectors, etc. between different coordinate systems (translations and rotations)

#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "geometry.h"


class Matrix4x4
{
  public:
    Matrix4x4( )  {} // default constructor --> identity matrix

  float m[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
};




class Transform
{
  public:
    // default constructor is identity matrix
    Transform( ) { }

    // constructor for translation matrix (defined by vector delta)
    Transform( const Vector delta )
    {
      matrix.m[0][3] = delta.x;  // --> x_trans = x + delta.x
      matrix.m[1][3] = delta.y;  // --> y_trans = x + delta.y
      matrix.m[2][3] = delta.z;  // --> z_trans = x + delta.z
    }

    // apply transforms to Points and Vectors
    Point operator()( const Point &p );
    Vector operator()( const Vector &p );
  
    Matrix4x4 matrix;
};


#endif  // _TRANSFORM_H_
