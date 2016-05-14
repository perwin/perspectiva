// Code for raytracing and saving images

#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

#include "vec3.h"
#include "geometry.h"
#include "scene.h"
#include "trace_options.h"
#include "render.h"


#if defined __linux__ || defined __APPLE__ 
// "Compiled for Linux
#else 
// Windows doesn't define these values by default, Linux does
#define M_PI 3.141592653589793 
#define INFINITY 1e8 
#endif 


// Local function definitions
float mix( const float &a, const float &b, const float &mix );

Vec3f ComputeCameraRay( float x, float y, float invWidth, float invHeight,
						float tanTheta, float aspectRatio );



// Used for computing Fresnel effects in Trace()
float mix( const float &a, const float &b, const float &mix )
{ 
  return b*mix + a*(1 - mix); 
} 


// This is the main trace function. It takes a ray as argument (defined by its origin
// and direction). We test if this ray intersects any of the geometry in the scene. 
// If the ray intersects an object, we compute the intersection point and the normal
// at the intersection point, and then shade this point using this information. 
// Shading depends on the surface property (is it transparent, reflective, diffuse). 
// The function returns a color for the ray. If the ray intersects an object, this is 
// the color of the object at the intersection point, otherwise it returns the background
// color.

Vec3f Trace( const Vec3f &rayorig, const Vec3f &raydir, const std::vector<Sphere> &objects, 
    		const int &depth )
{ 
  //if (raydir.length() != 1) std::cerr << "Error " << raydir << std::endl;
  float tnear = INFINITY; 
  const Sphere* sphere = NULL; 
  // find intersection of this ray with the sphere in the scene
  for (int i = 0; i < objects.size(); ++i) { 
    float t0 = INFINITY, t1 = INFINITY; 
    if (objects[i].intersect(rayorig, raydir, t0, t1)) { 
      if (t0 < 0) 
        t0 = t1; 
      if (t0 < tnear) { 
          tnear = t0; 
          sphere = &objects[i]; 
      } 
    } 
  } 
  // if there's no intersection, return black or background color
  if (!sphere) 
    return Vec3f(2); 
  
  Vec3f surfaceColor = 0;   // color of the ray/surfaceof the object intersected by the ray 
  Vec3f phit = rayorig + raydir*tnear;   // point of intersection 
  Vec3f nhit = phit - sphere->center;   // normal at the intersection point 
  nhit.normalize();   // normalize normal direction 
  // If the normal and the view direction are not opposite to each other, reverse 
  // the normal direction. That also means we are inside the sphere so set the inside 
  // bool to true. Finally reverse the sign of IdotN which we want positive.
  float bias = 1e-4; // add some bias to the point from which we will be tracing 
  bool inside = false; 
  if (raydir.dotProduct(nhit) > 0) 
    nhit = -nhit, inside = true; 
  if ((sphere->transparency > 0 || sphere->reflection > 0) && depth < MAX_RAY_DEPTH) { 
    float facingratio = -raydir.dotProduct(nhit); 
    // change the mix value to tweak the effect
    float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1); 
    // compute reflection direction (not need to normalize because all vectors
    // are already normalized)
    Vec3f refldir = raydir - nhit*2*raydir.dotProduct(nhit); 
    refldir.normalize(); 
    Vec3f reflection = Trace(phit + nhit*bias, refldir, objects, depth + 1); 
    Vec3f refraction = 0; 
    // if the sphere is also transparent compute refraction ray (transmission)
    if (sphere->transparency > 0) { 
      float ior = 1.1, eta = (inside) ? ior : 1 / ior; // are we inside or outside the surface? 
      float cosi = -nhit.dotProduct(raydir); 
      float k = 1 - eta*eta*(1 - cosi*cosi); 
      Vec3f refrdir = raydir*eta + nhit*(eta*cosi - sqrt(k)); 
      refrdir.normalize(); 
      refraction = Trace(phit - nhit*bias, refrdir, objects, depth + 1); 
    } 
    // the result is a mix of reflection and refraction (if the sphere is transparent)
    surfaceColor = (reflection*fresneleffect + 
      refraction*(1 - fresneleffect)*sphere->transparency) * sphere->surfaceColor; 
  } 
  else { 
    // it's a diffuse object, no need to raytrace any further
    for (int i = 0; i < objects.size(); ++i) { 
      Vec3f emissColor = objects[i].emissionColor;
      if ((emissColor.x > 0) || (emissColor.y > 0) || (emissColor.z > 0) ) { 
        // this is a light
        Vec3f transmission = 1; 
        Vec3f lightDirection = objects[i].center - phit; 
        lightDirection.normalize(); 
        for (int j = 0; j < objects.size(); ++j) { 
          if (i != j) { 
            float t0, t1; 
            if (objects[j].intersect(phit + nhit*bias, lightDirection, t0, t1)) { 
              transmission = 0; 
              break; 
            } 
          } 
        } 
        surfaceColor += sphere->surfaceColor*transmission * std::max(float(0), nhit.dotProduct(lightDirection))*objects[i].emissionColor; 
      } 
    } 
  } 

  return surfaceColor + sphere->emissionColor; 
}



// Save result to a PPM image (keep these flags if you compile under Windows)
// NOTE (PE): We can conver the ppm image to e.g. png using "convert" (ImageMagick):
//    $ convert untitled.ppm untitled.png
// or using GraphicsMagick "gm convert":
//    $ gm convert untitled.ppm untitled.png
void SaveImage( Vec3f *image, unsigned width, unsigned height, string imageFilename )
{
  string outputFilename = imageFilename + ".ppm";
  unsigned char r, g, b;
  
  std::ofstream ofs(outputFilename.c_str(), std::ios::out | std::ios::binary); 
  ofs << "P6\n" << width << " " << height << "\n255\n"; 
  for (int i = 0; i < width*height; ++i) {
    // PE: min(1, image[i].x) ensures that rgb values are always <= 1.0,
    // max(0, ...) ensures they are always >= 0.0.
    // These are then multiplied by 255 to get into the 0,255 range
    // + 0.5 does slightly nicer quantizing (values from 99.5--100.49 --> 100
    // instead of 99.01--100.0 --> 100
    r = (unsigned char)(std::max(0.f, std::min(1.f, image[i].x)) * 255 + 0.5);
    g = (unsigned char)(std::max(0.f, std::min(1.f, image[i].y)) * 255 + 0.5);
    b = (unsigned char)(std::max(0.f, std::min(1.f, image[i].z)) * 255 + 0.5);
    ofs << r << g << b;
  } 
  ofs.close(); 
  
  printf("Saved image file \"%s\".\n", outputFilename.c_str());
}



Vec3f ComputeCameraRay( float x, float y, float invWidth, float invHeight,
						float tanTheta, float aspectRatio )
{
  // We start with image-plane ("raster space") coordinate (x_pix,y_pix)
  // 1. convert these to normalized device coordinates (0--1,0--1); use center of 
  // each pixel (+ 0.5 pix):
  //    x_ndc = (x_pix + 0.5)/width
  //    y_ndc = (y_pix + 0.5)/height
  // 2. Remap to "screen space", which runs from -1 to +1:
  //    x_scrn = 2*x_ndc - 1
  //    y_scrn = 2*y_ndc - 1
  // 3. Since we want y_scrn to run from - to + as we go from bottom to top,
  // we need to invert the y_scrn values:
  //    y_scrn = 1 - 2*y_ndc
  //
  // Finally, correct for non-square aspect ratio (for x) and for field-of-view angle
  float  xx = (2*((x + 0.5)*invWidth) - 1.0) * tanTheta * aspectRatio;
  float  yy = (1.0 - 2*((y + 0.5)*invHeight)) * tanTheta;
  Vec3f raydir(xx, yy, -1.0); 
  raydir.normalize();
  return raydir;
}
  

Vec3f AverageSamples( vector<Vec3f> samples, const int nSamples )
{
  Vec3f cumulativeColor = Vec3f(0);
  float  scaling = 1.0 / nSamples;
  
  for (int i = 0; i < nSamples; ++i) {
    cumulativeColor += samples[i];
  }
  return (scaling * cumulativeColor);
}


// Main rendering function. We compute a camera ray for each pixel of the image, 
// trace it, and return a color. If the ray hits a sphere, we return the color of 
// the sphere at the intersection point, otherwise we return the background color.
void RenderAndSaveImage( Scene &theScene, unsigned width, unsigned height, 
			string filename, const traceOptions &options )
{ 
  // camera setup
  Vec3f *image = new Vec3f[width*height];
  Vec3f *pixel = image;
  Vec3f  pixelColor;
  float  invWidth = 1.0 / float(width);
  float  invHeight = 1.0 / float(height); 
  float  aspectRatio = width / float(height);
  Vec3f  cameraRayDir;
  
  float  fov = options.FieldOfView;
  float  tanTheta = tan(0.5*fov * M_PI/ 180.);
  
  bool  doOversampling = false;
  int  oversampleRate = 1;
  int  oversampleOffset;
  float  oversamplePixFrac;
  float  xx, yy, scaling;
  
  if (options.oversampling > 1) {
    doOversampling = true;
    oversampleRate = options.oversampling;
    oversamplePixFrac = 1.0 / oversampleRate;
    oversampleOffset = oversampleRate / 2 + 1 - oversampleRate;
    scaling = 1.0 / (oversampleRate * oversampleRate);
  }
  
  // Trace the rays
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (doOversampling) {
        Vec3f cumulativeColor = Vec3f(0);
        for (int j = 0; j < oversampleRate; ++j) {
          yy = y + oversamplePixFrac*(j + oversampleOffset);
          for (int i = 0; i < oversampleRate; ++i) {
            xx = x + oversamplePixFrac*(i + oversampleOffset);
            cameraRayDir = ComputeCameraRay(xx, yy, invWidth, invHeight, tanTheta, aspectRatio);
            cumulativeColor += Trace(Vec3f(0), cameraRayDir, theScene.objects, 0);
          }
        }
        pixelColor = cumulativeColor * scaling;
      }
      else {
        cameraRayDir = ComputeCameraRay(x, y, invWidth, invHeight, tanTheta, aspectRatio);
        pixelColor = Trace(Vec3f(0), cameraRayDir, theScene.objects, 0);
      }
      *pixel = pixelColor;
      ++pixel;
    } 
  }
  
  printf("Done with render.\n");
  SaveImage(image, width, height, filename);

  delete [] image; 
}
