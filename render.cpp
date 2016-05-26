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
#include "cameras.h"
#include "uniform_sampler.h"
#include "uniform_jitter_sampler.h"


// Local function definitions
float mix( const float &a, const float &b, const float &mix );



// Used for computing Fresnel effects in RayTrace()
float mix( const float &a, const float &b, const float &mix )
{
  return b*mix + a*(1.0 - mix);
}


bool TraceShadowRay( const Vec3f &lightDirection, const float lightDistance, 
					const std::vector<Object *> objects, const Vec3f &p_hit,
					const Vec3f &n_hit )
{
  bool blocked = false;
  // Check to see if another object is blocking path to light (shadow rays).
  // Note that we need to use -lightDirection, since we want the direction vector
  // *from* the object toward the light.
  // Also note that we assume blocking objects (between this point and the light)
  // are *opaque*; we are not attempting to handle transparent/translucent objects
  // (which, to be correct, would involve refraction and caustics...)
  for (int j = 0; j < objects.size(); ++j) {
    float t_0, t_1;
    if (objects[j]->intersect(p_hit + n_hit*BIAS, -lightDirection, &t_0, &t_1)) {
      // we intersected an object; check to see if it's closer to us than the light
      if ((t_0 < lightDistance) || (t_1 < lightDistance)) {
        blocked = true;
        break;
      }
    }
  }
  return blocked;
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
      reflectionColor = RayTrace(p_hit + n_hit*BIAS, refldir, theScene, depth + 1);
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
      refractionColor = RayTrace(p_hit - n_hit*BIAS, refrdir, theScene, depth + 1);
    }
    // the result is a mix of reflection and refraction
    surfaceColor = (reflectionColor*fresnelEffect +
      refractionColor*(1 - fresnelEffect)*intersectedObject->transparency) * intersectedObject->surfaceColor;
  }
  else {
    // it's a diffuse object, no need to raytrace any further; instead, trace
    // shadow rays to lights
    bool blocked;
    Color lightIntensity(0);
    Vec3f lightDirection;   // direction ray from point to light
    float lightDistance;
    for (int il = 0; il < lights.size(); ++il) {
      int nSamplesForLight = lights[il]->NSamples();
      for (int nn = 0; nn < nSamplesForLight; nn++) {
        // get a new shadow ray toward light
        lights[il]->illuminate(p_hit, lightDirection, lightIntensity, lightDistance);
        lightDirection.normalize();
        blocked = TraceShadowRay(lightDirection, lightDistance, objects, p_hit, n_hit);
        if (! blocked)
          surfaceColor += intersectedObject->surfaceColor * 
        					std::max(float(0), n_hit.dotProduct(-lightDirection)) * lightIntensity;
      }
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
  Color *pixel = image;
  Camera *theCamera = new Camera(options.FieldOfView, width, height);
  Vec3f  cameraRay;
  Sampler *sampler;  
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
        cameraRay = theCamera->GenerateCameraRay(xx, yy);
        //printf("image x,y = %f,%f\n", xx,yy);
        cumulativeColor += RayTrace(Vec3f(0), cameraRay, theScene, 0);
      }
      *pixel = cumulativeColor * scaling;
      ++pixel;
    } 
  }
  
  printf("Done with render.\n");
  
  delete sampler;
  delete theCamera;
}
