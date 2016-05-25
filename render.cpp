// Code for raytracing images

#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

#include "definitions.h"
#include "vec3.h"
#include "geometry.h"
#include "lights.h"
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

Vec3f GenerateCameraRay( float x, float y, float invWidth, float invHeight,
						float tanTheta, float aspectRatio );



// Used for computing Fresnel effects in RayTrace()
float mix( const float &a, const float &b, const float &mix )
{
  return b*mix + a*(1.0 - mix);
}



/// Generates a normalized direction ray for the camera (starting from
/// pinhole aperture, heading out through current image-plane pixel (x,y),
/// assuming camera characteristics tanTheta and image-plane characteristics,
/// invWidth, invHeight, and aspectRatio
Vec3f GenerateCameraRay( float x, float y, float tanTheta, float invWidth, 
						float invHeight, float aspectRatio )
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
  


// This is the main trace function. It takes a ray as argument (defined by its origin
// and direction). We test if this ray intersects any of the geometry in the scene. 
// If the ray intersects an object, we compute the intersection point and the normal
// at the intersection point, and then shade this point using this information. 
// Shading depends on the surface property (is it transparent, reflective, diffuse). 
// The function returns a color for the ray. If the ray intersects an object, this is 
// the color of the object at the intersection point, otherwise it returns the background
// color.
Color RayTrace( const Vec3f &rayorig, const Vec3f &raydir, Scene *theScene, 
    		const int depth )
{
  std::vector<Object *> objects = theScene->objects;
  std::vector<Light *> lights = theScene->lights;
  Color  backgroundColor = theScene->backgroundColor;
  
  float t_nearest = INFINITY;
  const Object* intersectedObject = NULL;
  // find intersection of this ray with objects in the scene
  int  intersectedObjIndex = -1;
  for (int i = 0; i < objects.size(); ++i) {
    float t_0 = INFINITY;
    float t_1 = INFINITY;
    if (objects[i]->intersect(rayorig, raydir, &t_0, &t_1)) {
      if (t_0 < 0)  // first intersection is *behind* camera, so use the second
        t_0 = t_1;
      if (t_0 < t_nearest) {
          // this object is closest of all so far
          t_nearest = t_0;
          intersectedObject = objects[i];
          intersectedObjIndex = i;
      }
    }
  }
  // if there's no intersection, return background color
  if (! intersectedObject)
    return backgroundColor;
  
  Color surfaceColor = 0;   // color of the surface of the object intersected by the ray
  Vec3f p_hit = rayorig + raydir*t_nearest;   // intersection point
  Vec3f n_hit = intersectedObject->GetNormalAtPoint(p_hit);   // normal at intersection point
  n_hit.normalize();   // normalize normal direction
  // If the normal and the view direction are not opposite to each other, reverse 
  // the normal direction. That also means we are e.g. inside a sphere, so set the inside 
  // bool to true. Finally reverse the sign of IdotN which we want positive.
  float bias = 1e-4;   // add some bias to the point from which we will be tracing 
  bool inside = false;
  if (raydir.dotProduct(n_hit) > 0)
    n_hit = -n_hit, inside = true;
  if ((intersectedObject->reflection > 0 || intersectedObject->transparency > 0) 
  		&& depth < MAX_RAY_DEPTH) {
  	// OK we need to generate reflection and/or refraction rays
    // change the mix value to tweak the effect
    float facingratio = -raydir.dotProduct(n_hit);
    float fresnelEffect = mix(pow(1 - facingratio, 3), 1, 0.1);
    Color reflectionColor = 0;
    // if the object is reflective, compute reflection ray
    if (intersectedObject->reflection > 0) {
      Vec3f refldir = raydir - n_hit*2*raydir.dotProduct(n_hit);  // reflection direction
      refldir.normalize();
      reflectionColor = RayTrace(p_hit + n_hit*bias, refldir, theScene, depth + 1);
    }
    Color refractionColor = 0;
    // if the object is transparent, compute refraction ray (transmission)
    if (intersectedObject->transparency > 0) {
      float ior = 1.1;
      float eta = (inside) ? ior : 1 / ior; // are we inside or outside the surface?
      float cosi = -n_hit.dotProduct(raydir);
      float k = 1 - eta*eta*(1 - cosi*cosi);
      Vec3f refrdir = raydir*eta + n_hit*(eta*cosi - sqrt(k));  // refraction direction
      refrdir.normalize();
      refractionColor = RayTrace(p_hit - n_hit*bias, refrdir, theScene, depth + 1);
    }
    // the result is a mix of reflection and refraction
    surfaceColor = (reflectionColor*fresnelEffect +
      refractionColor*(1 - fresnelEffect)*intersectedObject->transparency) * intersectedObject->surfaceColor;
  }
  else {
    // it's a diffuse object, no need to raytrace any further; instead, trace
    // shadow rays to lights
    for (int il = 0; il < lights.size(); ++il) {
      Color transmission = 1;
      Color lightIntensity(0);
      Vec3f lightDirection;   // direction ray from point to light
      float  lightDistance;
      lights[il]->illuminate(p_hit, lightDirection, lightIntensity, lightDistance);
      lightDirection.normalize();
      // Check to see if another object is blocking path to light (shadow rays).
      // Note that we need to use -lightDirection, since we want the direction vector
      // *from* the object toward the light.
      // Also note that we assume blocking objects (between this point and the light)
      // are *opaque*; we are not attempting to handle transparent/translucent objects
      // (which, to be correct, would involve refraction and caustics...)
      for (int j = 0; j < objects.size(); ++j) {
        float t_0, t_1;
        if ((j != intersectedObjIndex) && 
        	(objects[j]->intersect(p_hit + n_hit*bias, -lightDirection, &t_0, &t_1))) {
          // we intersected an object; check to see if it's closer to us than the light
          if ((t_0 < lightDistance) || (t_1 < lightDistance)) {
            transmission = 0;
            break;
          }
        }
      }
      surfaceColor += intersectedObject->surfaceColor * transmission *
        				std::max(float(0), n_hit.dotProduct(-lightDirection)) * lightIntensity;
    }
  }

  return surfaceColor + intersectedObject->emissionColor;
}



// Main rendering function. We compute a camera ray for each pixel of the image, 
// trace it, and return a color. If the ray hits a sphere, we return the color of 
// the sphere at the intersection point, otherwise we return the background color.
void RenderImage( Scene *theScene, Color *image, int width, int height, 
				const traceOptions &options )
{
  // camera setup
  Color *pixel = image;
  float  invWidth = 1.0 / float(width);
  float  invHeight = 1.0 / float(height);
  float  aspectRatio = width / float(height);
  Vec3f  cameraRay;
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
      Color cumulativeColor = Color(0);
      for (int n = 0; n < nSubsamples; ++n) {
        sampler->GetOffsetCoords(n, &xOff, &yOff);
        xx = x + xOff;
        yy = y + yOff;
        cameraRay = GenerateCameraRay(xx, yy, tanTheta, invWidth, invHeight, aspectRatio);
        //printf("image x,y = %f,%f\n", xx,yy);
        cumulativeColor += RayTrace(Vec3f(0), cameraRay, theScene, 0);
      }
      *pixel = cumulativeColor * scaling;
      ++pixel;
    } 
  }
  
  printf("Done with render.\n");
  
  delete sampler;
}
