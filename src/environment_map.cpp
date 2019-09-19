
#include "environment_map.h"


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

