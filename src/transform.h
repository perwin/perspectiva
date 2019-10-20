// Code for 4x4 matrices using homogeneous coordinates for transforming 
// Points, Vectors, etc. between different coordinate systems (translations and rotations)

#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "geometry.h"


class Matrix4x4
{
  public:
    Matrix4x4( )  {} // default constructor --> identity matrix

//     Matrix4x4 operator=( const Matrix4x4 &input )
//     {
//       m[0][0] = input[0][0];
//       m[0][1] = input[0][1];
//       m[0][2] = input[0][2];
//       m[0][3] = input[0][3];
//       m[1][0] = input[1][0];
//       m[1][1] = input[1][1];
//       m[1][2] = input[1][2];
//       m[1][3] = input[1][3];
//       m[2][0] = input[2][0];
//       m[2][1] = input[2][1];
//       m[2][2] = input[2][2];
//       m[2][3] = input[2][3];
//       m[3][0] = input[3][0];
//       m[3][1] = input[3][1];
//       m[3][2] = input[3][2];
//       m[3][3] = input[3][3];
//     }

  float m[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
};




class Transform
{
  public:
    // default constructor is identity matrix
    Transform( ) { }

    Transform( const Matrix4x4 input )
    {
      matrix = input;
//       matrix.m[0][0] = input[0][0];
//       matrix.m[0][1] = input[0][1];
//       matrix.m[0][2] = input[0][2];
//       matrix.m[0][3] = input[0][3];
//       matrix.m[1][0] = input[1][0];
//       matrix.m[1][1] = input[1][1];
//       matrix.m[1][2] = input[1][2];
//       matrix.m[1][3] = input[1][3];
    }

    // constructor for pure translation matrix (defined by vector delta)
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



// Code for making specialized transforms

Transform RotateX( float angle );

Transform RotateY( float angle );

Transform RotateZ( float angle );


#endif  // _TRANSFORM_H_
