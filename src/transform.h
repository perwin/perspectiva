// Code for 4x4 matrices using homogeneous coordinates for transforming 
// Points, Vectors, etc. between different coordinate systems (translations and rotations)

#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <string.h>
#include "geometry.h"


class Matrix4x4
{
  public:
    Matrix4x4( )  {} // default constructor --> identity matrix

    Matrix4x4( float mat[4][4] )
    {
      memcpy(m, mat, 16*sizeof(float));
    }

    Matrix4x4 operator*( const Matrix4x4 &b ) const
    {
      Matrix4x4  c;
      for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
          c.m[i][j] = m[i][0] * b.m[0][j] + m[i][1] * b.m[1][j] + 
                  		m[i][2] * b.m[2][j] + m[i][3] * b.m[3][j];
        }
      return c;
    }
  
    float m[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
};



Matrix4x4 InvertMatrix( const Matrix4x4 &m );



class Transform
{
  public:
    // default constructor is identity matrix
    Transform( ) { }

    Transform( const Matrix4x4 input )
    {
      matrix = input;
      invMatrix = InvertMatrix(matrix);
    }

    // constructor for pure translation matrix (defined by vector delta)
    Transform( const Vector delta )
    {
      matrix.m[0][3] = delta.x;  // --> x_trans = x + delta.x
      matrix.m[1][3] = delta.y;  // --> y_trans = x + delta.y
      matrix.m[2][3] = delta.z;  // --> z_trans = x + delta.z
      invMatrix = InvertMatrix(matrix);
    }

    // multiply transforms
    Transform operator*( const Transform &t2 ) const
    {
      return Transform(matrix * t2.matrix);
    }
    
    // apply transforms to Points and Vectors
    Point operator()( const Point &p );
    Vector operator()( const Vector &p );
  
    Matrix4x4 matrix;
    Matrix4x4 invMatrix;
};



// Code for making specialized transforms

Transform RotateX( float angle );

Transform RotateY( float angle );

Transform RotateZ( float angle );


#endif  // _TRANSFORM_H_
