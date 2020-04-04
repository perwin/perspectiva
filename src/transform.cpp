// Code for Transform class

#include "transform.h"




// Member functions to apply transforms to Points and Vectors
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



// Auxiliary functions to create specialized transforms
Transform RotateX( float angle )
{
  float cosTheta = cos(angle);
  float sinTheta = sin(angle);
  Matrix4x4 matrix;
  matrix.m[1][1] = cosTheta;
  matrix.m[1][2] = -sinTheta;
  matrix.m[2][1] = sinTheta;
  matrix.m[2][2] = cosTheta;
  return Transform(matrix);
}


Transform RotateY( float angle )
{
  float cosTheta = cos(angle);
  float sinTheta = sin(angle);
  Matrix4x4 matrix;
  matrix.m[0][0] = cosTheta;
  matrix.m[0][2] = sinTheta;
  matrix.m[2][0] = -sinTheta;
  matrix.m[2][2] = cosTheta;
  return Transform(matrix);
}


Transform RotateZ( float angle )
{
  float cosTheta = cos(angle);
  float sinTheta = sin(angle);
  Matrix4x4 matrix;
  matrix.m[0][0] = cosTheta;
  matrix.m[0][1] = -sinTheta;
  matrix.m[1][0] = sinTheta;
  matrix.m[1][1] = cosTheta;
  return Transform(matrix);
}


// Matrix inversion (code from PBRT), using Gauss-Jordan elimination
Matrix4x4 InvertMatrix( const Matrix4x4 &m )
{
  int  indxc[4], indxr[4];
  int  ipiv[4] = {0, 0, 0, 0};
  float  minv[4][4];
  memcpy(minv, m.m, 4 * 4 * sizeof(float));
  for (int i = 0; i < 4; i++) {
    int  irow = 0, icol = 0;
    float  big = 0.f;
    // Choose pivot
    for (int j = 0; j < 4; j++) {
      if (ipiv[j] != 1) {
        for (int k = 0; k < 4; k++) {
          if (ipiv[k] == 0) {
            if (fabsf(minv[j][k]) >= big) {
              big = float(fabsf(minv[j][k]));
              irow = j;
              icol = k;
            }
          } else if (ipiv[k] > 1) {
            fprintf(stderr, "Singular matrix in InvertMatrix()");
            exit(1);
          }
        }
      }
    }
    ++ipiv[icol];
    // Swap rows _irow_ and _icol_ for pivot
    if (irow != icol) {
      for (int k = 0; k < 4; ++k)
        std::swap(minv[irow][k], minv[icol][k]);
    }
    indxr[i] = irow;
    indxc[i] = icol;
    if (minv[icol][icol] == 0.f) {
      fprintf(stderr, "Singular matrix in InvertMatrix()");
      exit(1);
    }

    // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
    float  pivinv = 1.0 / minv[icol][icol];
    minv[icol][icol] = 1.0;
    for (int j = 0; j < 4; j++)
      minv[icol][j] *= pivinv;

    // Subtract this row from others to zero out their columns
    for (int j = 0; j < 4; j++) {
      if (j != icol) {
        float save = minv[j][icol];
        minv[j][icol] = 0;
        for (int k = 0; k < 4; k++)
          minv[j][k] -= minv[icol][k] * save;
      }
    }
  }
  // Swap columns to reflect permutation
  for (int j = 3; j >= 0; j--) {
    if (indxr[j] != indxc[j]) {
      for (int k = 0; k < 4; k++)
        std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
    }
  }
  return Matrix4x4(minv);
}
