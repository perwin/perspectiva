// Code for camera classes.
// Based originally on code from scratchapixel.com

#ifndef _CAMERAS_H_
#define _CAMERAS_H_

#include <cstdlib> 
#include "geometry.h"
#include "color.h"
#include "definitions.h"
#include "sampler.h"
#include "uniform_sampler.h"
#include "uniform_jitter_sampler.h"
#include "render_utils.h"


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
    tanTheta = tan(0.5*fieldOfView*DEG2RAD);
    
    // default -- no depth-of-field
    focalDistance = kInfinity;
    apertureRadius = 0.0;   // pinhole
        
    cameraType = CAMERA_PERSPECTIVE;
  };

  ~Camera() 
  {
    if (samplerAllocated)
      delete sampler;
  };


  /// Generates a normalized direction ray for the camera (starting from the
  /// origin (0,0,0), corresponding to the pinhole aperture (or the center of
  /// the lens aperture if using depth of field), heading out through current
  /// image-plane pixel (x,y), assuming camera characteristics tanTheta and
  /// image-plane characteristics, invWidth, invHeight, and aspectRatio.
  /// If subsampling is being done, then the camera object's Sampler instance
  /// is called to produce appropriate image-plane sub-pixel offsets for x and y,
  /// for the current subsampleNumber; these are returned in x_out and y_out.
  Ray GenerateCameraRay( float x_pix, float y_pix, int subsampleNumber,
  							float *x_out, float *y_out )
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
    float  xOff, yOff, x, y;
    sampler->GetOffsetCoords(subsampleNumber, &xOff, &yOff);
    x = x_pix + xOff;
    y = y_pix + yOff;

    float  x_world = (2*((x + 0.5)*invWidth) - 1.0) * tanTheta * aspectRatio;
    float  y_world = (1.0 - 2*((y + 0.5)*invHeight)) * tanTheta;

	Ray cameraRay(Point(0), Point(x_world, y_world, -1.0), 0);
    *x_out = x;
    *y_out = y;
	return cameraRay;
  }
  
  /// Generate a random point within the idealized thin lens
  Point GenerateLensPoint( )
  {
    // the reference point which we offset from is always the nominal pinhole/origin 
    // point at (0,0,0)
    Point origin(0);
    Vector offset = apertureRadius * UnitDisk_RejectionSample();
    return origin + offset;
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
    tanTheta = tan(0.5*fieldOfView*DEG2RAD);
  };

  void SetFocalDistance( float dist )
  { 
    focalDistance = dist;
  };

  void SetAperture( float radius )
  { 
    apertureRadius = radius;
  };

  void SetSampling( int oversampling, const std::string &oversamplerName )
  {
    oversampleRate = oversampling;
    if (oversamplerName == SAMPLER_UNIFORM)
      sampler = new UniformSampler(oversampleRate);
    else if (oversamplerName == SAMPLER_UNIFORM_JITTER)
      sampler = new UniformJitterSampler(oversampleRate);
    samplerAllocated = true;
  };
  
  void UpdateSampler( )
  {
    sampler->Update();
  };
  
  int GetType( )
  { 
    return cameraType; 
  };
 
  // data members
  int  cameraType;
  Point position;  // in world coordinates
  Vector direction;  // in world coordinates
  float fieldOfView;  // degrees
  float tanTheta, invWidth, invHeight;
  float aspectRatio;
  float focalDistance;
  float apertureRadius;
  bool samplerAllocated = false;
  int oversampleRate = 1;
  int nSubsamples = 1;
  Sampler *sampler;  
};



#endif  // _CAMERAS_H_
