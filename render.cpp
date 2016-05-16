// Code for raytracing images

#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

#include "definitions.h"
#include "vec3.h"
#include "geometry.h"
#include "scene.h"
#include "option_structs.h"
#include "render.h"
#include "sampler.h"
#include "uniform_sampler.h"
#include "uniform_jitter_sampler.h"


#if defined __linux__ || defined __APPLE__
// "Compiled for Linux
#else
// Windows doesn't define these values by default, Linux does
#define M_PI 3.141592653589793
#define INFINITY 1e8
#endif

const float DEG2RAD = M_PI / 180.0;


// Local function definitions
float mix( const float &a, const float &b, const float &mix );

Vec3f ComputeCameraRay( float x, float y, float invWidth, float invHeight,
						float tanTheta, float aspectRatio );



// Used for computing Fresnel effects in RayTrace()
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
Vec3f RayTrace( const Vec3f &rayorig, const Vec3f &raydir, const std::vector<Sphere> &objects, 
    		const int &depth )
{
  //if (raydir.length() != 1) std::cerr << "Error " << raydir << std::endl;
  float tnear = INFINITY;
  const Sphere* intersectedObject = NULL;
  // find intersection of this ray with the sphere in the scene
  for (int i = 0; i < objects.size(); ++i) {
    float t0 = INFINITY;
    float t1 = INFINITY;
    if (objects[i].intersect(rayorig, raydir, &t0, &t1)) {
      if (t0 < 0)
        t0 = t1;
      if (t0 < tnear) {
          // this object is closest of all so far
          tnear = t0;
          intersectedObject = &objects[i];
      }
    }
  }
  // if there's no intersection, return black or background color
  if (! intersectedObject)
    return Vec3f(2);
  
  Vec3f surfaceColor = 0;   // color of the ray/surface of the object intersected by the ray
  Vec3f phit = rayorig + raydir*tnear;   // point of intersection
  Vec3f nhit = phit - intersectedObject->center;   // normal at the intersection point
  nhit.normalize();   // normalize normal direction
  // If the normal and the view direction are not opposite to each other, reverse 
  // the normal direction. That also means we are inside the sphere so set the inside 
  // bool to true. Finally reverse the sign of IdotN which we want positive.
  float bias = 1e-4;   // add some bias to the point from which we will be tracing 
  bool inside = false;
  if (raydir.dotProduct(nhit) > 0)
    nhit = -nhit, inside = true;
  if ((intersectedObject->transparency > 0 || intersectedObject->reflection > 0) && depth < MAX_RAY_DEPTH) {
    float facingratio = -raydir.dotProduct(nhit);
    // change the mix value to tweak the effect
    float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);
    // compute reflection direction (not need to normalize because all vectors
    // are already normalized)
    Vec3f refldir = raydir - nhit*2*raydir.dotProduct(nhit);
    refldir.normalize();
    Vec3f reflection = RayTrace(phit + nhit*bias, refldir, objects, depth + 1);
    Vec3f refraction = 0;
    // if the sphere is also transparent compute refraction ray (transmission)
    if (intersectedObject->transparency > 0) {
      float ior = 1.1, eta = (inside) ? ior : 1 / ior; // are we inside or outside the surface?
      float cosi = -nhit.dotProduct(raydir);
      float k = 1 - eta*eta*(1 - cosi*cosi);
      Vec3f refrdir = raydir*eta + nhit*(eta*cosi - sqrt(k));
      refrdir.normalize();
      refraction = RayTrace(phit - nhit*bias, refrdir, objects, depth + 1);
    }
    // the result is a mix of reflection and refraction (if the sphere is transparent)
    surfaceColor = (reflection*fresneleffect +
      refraction*(1 - fresneleffect)*intersectedObject->transparency) * intersectedObject->surfaceColor;
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
            if (objects[j].intersect(phit + nhit*bias, lightDirection, &t0, &t1)) {
              transmission = 0;
              break;
            }
          }
        }
        surfaceColor += intersectedObject->surfaceColor * transmission *
        				std::max(float(0), nhit.dotProduct(lightDirection)) *
        				objects[i].emissionColor;
      }
    }
  }

  return surfaceColor + intersectedObject->emissionColor;
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
  


// Main rendering function. We compute a camera ray for each pixel of the image, 
// trace it, and return a color. If the ray hits a sphere, we return the color of 
// the sphere at the intersection point, otherwise we return the background color.
void RenderImage( Scene &theScene, Vec3f *image, int width, int height, 
			const traceOptions &options )
{
  // camera setup
  Vec3f *pixel = image;
  float  invWidth = 1.0 / float(width);
  float  invHeight = 1.0 / float(height);
  float  aspectRatio = width / float(height);
  Vec3f  cameraRayDir;
  Sampler *sampler;
  
  float  fov = options.FieldOfView;
  float  tanTheta = tan(0.5*fov*DEG2RAD);
  
  int  oversampleRate = 1;
  float  xx, yy, scaling;
  float  xOff, yOff;
  int  nSubsamples;
  
  // setup for oversampling
  oversampleRate = options.oversampling;
  nSubsamples = oversampleRate*oversampleRate;
  scaling = 1.0 / (oversampleRate * oversampleRate);
  if (options.samplerName == SAMPLER_UNIFORM)
    sampler = new UniformSampler(oversampleRate);
  else if (options.samplerName == SAMPLER_UNIFORM_JITTER)
    sampler = new UniformJitterSampler(oversampleRate);
  
  // Trace the rays, with possible per-pixel oversampling
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      sampler->Update();
      Vec3f cumulativeColor = Vec3f(0);
      for (int n = 0; n < nSubsamples; ++n) {
        sampler->GetOffsetCoords(n, &xOff, &yOff);
        xx = x + xOff;
        yy = y + yOff;
        cameraRayDir = ComputeCameraRay(xx, yy, invWidth, invHeight, tanTheta, aspectRatio);
        cumulativeColor += RayTrace(Vec3f(0), cameraRayDir, theScene.objects, 0);
      }
      *pixel = cumulativeColor * scaling;
      ++pixel;
    } 
  }
  
  printf("Done with render.\n");
  
  delete sampler;
}
