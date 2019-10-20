
#include "environment_map.h"
#include "definitions.h"


// const float PI_OVER_TWO = 1.5707963267948966;
// // const float PI = 3.141592653589793;
// const float TWO_PI = 6.283185307179586;

// OK, this is a bit confusing. The convention for cube mapping is the Renderman
// convention, also used by OpenGL, which uses a *left-handed* coordinate system with 
// its origin in the *center* of the cube. The "front" face is pointed to by the
// +z vector (which corresponds to the -z vector in the rest of 3D); the "back"
// face is pointed to by the -z vector (corresponding to +z in normal 3D orientation).
// "Top" and "bottom" are pointed to by the +y and -y vectors.
// 
// Second, the Renderman/OpenGL convention specifies that an individual image starts at
// the upper-left corner, not the lower-left corner. Thus, we need to treat the v
// coordinate as -y
// 
// 
// https://stackoverflow.com/questions/11685608/convention-of-faces-in-opengl-cubemapping

// The following works, for the case where the images are laid out as shown below,
// using the Renderman/OpenGL left-hand coordinate convention (for inside the box,
// looking out):
// 
//           ________
//          |        |
//          | pos y  |
//          | TOP    |
//   _______|________|_________________
//  |       |        |        |        |
//  | neg x | pos z  |  pos x |  neg z |
//  | LEFT  | FRONT  |  RIGHT |  BACK  |
//  |_______|________|________|________|
//          |        |
//          | BOTTOM |
//          | neg y  |
//          |________|

// indexing of images
// 0 = right, 1 = left, 2 = top, 3 = bottom, 4 = back, 5 = front

// Based on code from https://en.wikipedia.org/wiki/Cube_mapping
void GetUVforCube( Ray theRay, int *imageIndex, float *u, float *v )
{
  float x = theRay.dir.x;
  float y = theRay.dir.y;
  float z = theRay.dir.z;
  float absX = fabs(x);
  float absY = fabs(y);
  float absZ = fabs(z);
  
  bool isXPositive = x > 0 ? true : false;
  bool isYPositive = y > 0 ? true : false;
  bool isZPositive = z > 0 ? true : false;
  
  float maxAxis, uc, vc;
  
  // POSITIVE X = Right face
  if (isXPositive && absX >= absY && absX >= absZ) {
    // u (0 to 1) goes from +z to -z
    // v (0 to 1) goes from -y to +y
    maxAxis = absX;
    uc = -z;
    vc = -y;   // Renderman/OpenGL convention
    *imageIndex = RIGHT_FACE;
  }
  // NEGATIVE X = Left face
  if (!isXPositive && absX >= absY && absX >= absZ) {
    // u (0 to 1) goes from -z to +z
    // v (0 to 1) goes from -y to +y
    maxAxis = absX;
    uc = z;
    vc = -y;   // Renderman/OpenGL convention
    *imageIndex = LEFT_FACE;
  }
  // POSITIVE Y = Top/Up face
  if (isYPositive && absY >= absX && absY >= absZ) {
    // u (0 to 1) goes from -x to +x
    // v (0 to 1) goes from +z to -z
    maxAxis = absY;
    uc = x;  // works!
    vc = z;  // works!
    *imageIndex = TOP_FACE;
  }
  // NEGATIVE Y = Bottom/Down face
  if (!isYPositive && absY >= absX && absY >= absZ) {
    // u (0 to 1) goes from -x to +x
    // v (0 to 1) goes from -z to +z
    maxAxis = absY;
    uc = x;
    vc = z;
    *imageIndex = BOTTOM_FACE;
  }
  // POSITIVE Z = Back face (This is Renderman/OpenGL +Z; normal 3D -Z)
  if (isZPositive && absZ >= absX && absZ >= absY) {
    // u (0 to 1) goes from -x to +x
    // v (0 to 1) goes from -y to +y
    maxAxis = absZ;
    uc = x;
    vc = -y;   // Renderman/OpenGL convention
    *imageIndex = BACK_FACE;
  }
  // NEGATIVE Z = Front face (This is Renderman/OpenGL -Z; normal 3D +Z)
  if (!isZPositive && absZ >= absX && absZ >= absY) {
    // u (0 to 1) goes from +x to -x
    // v (0 to 1) goes from -y to +y
    maxAxis = absZ;
    uc = -x;
    vc = -y;   // Renderman/OpenGL convention
    *imageIndex = FRONT_FACE;
  }

  // Convert range from -1 to 1 to 0 to 1
  *u = 0.5f * (uc / maxAxis + 1.0f);
  *v = 0.5f * (vc / maxAxis + 1.0f);
}


// Assume following equiangular defintion (assuming z-axis is flipped)
//    x=1, z=0 ==> longitude = 270 W = 90 E = 3 pi/2 = -pi/2
//    x=0, z=1 ==> longitude = 0 [prime meridian]
//    x=-1,z=0 ==> longitude = 90 W = pi/2
//    x=0, z=-1 ==> longitude = 180 W/E = pi

// angles increase CCW, like normal (so "longitude" is degrees *west* on globe!)
// left side of image: x=-1,z=0 ==> long = 180 W = +pi
// right side of image: x=1,z=0 ==> long = -180 W = -pi


// atan2(z, x) --> -pi,pi


void GetUVforSphere( Ray theRay, float *u, float *v, float longitudeRotation )
{
  float x = theRay.dir.x;
  float y = theRay.dir.y;
  // flip z, so +z points out along camera axis
  float z = -theRay.dir.z;

  // determine azimuth (longitude); convert so longitude = 0 points
  // along z-axis, not x-axis
  float longitude = atan2f(z, x) - PI_OVER_TWO - longitudeRotation;
  if (longitude <= -PI)
    longitude += TWO_PI;
  
  // determine polar angle (latitude): -pi/2 to pi/2
  // note that we need to take negative of latitude, since image coordinates
  // start at upper left of image
  float r_plane = sqrtf(x*x + z*z);
  float latitude = -atan2f(y, r_plane);
  
  *u = (PI - longitude) / TWO_PI;
  *v = (latitude + PI_OVER_TWO) / PI;
}

