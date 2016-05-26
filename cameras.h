// Code for camera classes; based on code from scratchapixel.com

#ifndef _CAMERAS_H_
#define _CAMERAS_H_

#include <cstdlib> 
#include "vec3.h"
#include "color.h"
#include "definitions.h"


// For now, we'll make this a concrete class; later we can make it into an
// abstract base class and have subclasses.


class Camera
{
public:
  Camera( float fov, int width, int height )
  {
    fieldOfView = fov;
    invWidth = 1.0 / float(width);
    invHeight = 1.0 / float(height);
    aspectRatio = width / float(height);
    tanTheta = tan(0.5*fov*DEG2RAD);
    
    cameraType = 0;
  };

  ~Camera() {};

/// Generates a normalized direction ray for the camera (starting from
/// pinhole aperture, heading out through current image-plane pixel (x,y),
/// assuming camera characteristics tanTheta and image-plane characteristics,
/// invWidth, invHeight, and aspectRatio
  Vec3f GenerateCameraRay( float x, float y )
  {
    // We start with a 2D image-plane ("raster space") coordinate (x_pix,y_pix); we want
    // to convert this to a coordinate in 3D camera space.
    //
    // 1. convert these to normalized device coordinates (0--1,0--1); use center of 
    // each pixel (+ 0.5 pix):
    //    x_ndc = (x_pix + 0.5)/width
    //    y_ndc = (y_pix + 0.5)/height
    // 2. Remap to "screen space", which runs from -1 to +1; since we want y_scrn to
    // run from - to + as we go from bottom to top, we need to invert the y_scrn values:
    //    x_scrn = 2*x_ndc - 1
    //    y_scrn = -(2*y_ndc - 1) = 1 - 2*y_ndc
    //
    // 3. Finally, transform to camera coordinates (with image plane at z = -1) and
    // correct for non-square aspect ratio (for x)
    //    x_cam = x_scrn * thanTheta * aspectRati
    //    y_cam = y_scrn * thanTheta
    //    z_cam = -1
    float  x_world = (2*((x + 0.5)*invWidth) - 1.0) * tanTheta * aspectRatio;
    float  y_world = (1.0 - 2*((y + 0.5)*invHeight)) * tanTheta;
    Vec3f raydir(x_world, y_world, -1.0);
    raydir.normalize();
    return raydir;
  }
  
  void SetImageSize( int width, int height )
  { 
    invWidth = 1.0 / float(width);
    invHeight = 1.0 / float(height);
    aspectRatio = width / float(height);
  };

  void SetFOV( float FOV )
  { 
    fieldOfView = FOV; 
  };

  int GetType( )
  { 
    return cameraType; 
  };

  // data members
  int  cameraType;
  Vec3f position, direction;  // in world coordinates
  float fieldOfView;  // degrees
  float tanTheta, invWidth, invHeight;
  float aspectRatio;
};



#endif  // _LIGHTS_H_
