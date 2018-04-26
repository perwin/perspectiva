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
#include "cameras.h"


// Local function definitions

// Used for computing Fresnel effects in RayTrace()
float mix( const float a, const float b, const float mix )
{
  return b*mix + a*(1.0 - mix);
}


// Given a point p_hit with surface normal n_hit and a direction vector 
// lightDirection to some light at a distance of lightDistance along the vector, 
// determine whether any objects are in between the point and the light.
bool TraceShadowRay( const Vec3f &lightDirection, const float lightDistance, 
					const std::vector<Object *> objects, const Vec3f &p_hit,
					const Vec3f &n_hit )
{
  bool blocked = false;
  // Check to see if another object is blocking path to light (shadow rays).
  // We use the same basic algorithm as for camera raytracing, which means that
  // lightDirection must be the direction ray from the shaded point *to*
  // the light. (So in most cases this function should be called with -lightDir
  // where lightDir is direction ray from light to point produced by an light's
  // Illuminate method.
  // Also note that we assume blocking objects (between this point and the light)
  // are *opaque*; we are not attempting to handle transparent/translucent objects
  // (which, to be correct, would involve refraction and caustics...)
  for (int j = 0; j < objects.size(); ++j) {
    float t_0, t_1;
    if (objects[j]->intersect(p_hit + n_hit*BIAS, lightDirection, &t_0, &t_1)) {
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
    			const int depth, float *t, const float x=0.f, const float y=0.f )
{
  std::vector<Object *> objects = theScene->objects;
  std::vector<Light *> lights = theScene->lights;
  Color  backgroundColor = theScene->backgroundColor;
  float  t_newRay;  // will hold distance to intersection of any reflection or
                    // transmission rays launched by this function
  
  float t_nearest = INFINITY;
  Object* intersectedObject = NULL;
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
  *t = t_nearest;
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
      reflectionColor = RayTrace(p_hit + n_hit*BIAS, refldir, theScene, depth + 1, &t_newRay);
    }
    Color refractionColor = 0;
    // if the object is transparent, compute refraction ray (transmission)
    if (intersectedObject->transparency > 0) {
      float ior = 1.1;   // TEMP IOR VALUE -- SHOULD GET THIS FROM OBJECT
      float eta = (inside) ? ior : 1 / ior; // are we inside or outside the surface?
      float cosi = -n_hit.dotProduct(raydir);
      float k = 1 - eta*eta*(1 - cosi*cosi);
      Vec3f refrdir = raydir*eta + n_hit*(eta*cosi - sqrt(k));  // refraction direction
      refrdir.normalize();
      refractionColor = RayTrace(p_hit - n_hit*BIAS, refrdir, theScene, depth + 1, &t_newRay);
      // possible application of Beer's Law:
      //    if object material includes attenuation of transmitted light:
      //      if refraction ray goes *into* the surface, get distance t = t_newRay traveled by
      //      ray, then multiply (L_r,L_g,L_b) by exp(-k_r*t), exp(-k_g*t), exp(-k_b*t)
    }
    // the final result is a mix of contributions from reflection and refraction
    // FIXME:
    //    Right now, we're using the Scratchapixel "very basic raytracer" approach, in 
    // which the reflection is unmodulated by any surface reflectivity value
    // (though it is multiplied by the Fresnel K_r value), while the refraction is
    // multiplied by both the Fresnel (1 - K_r) *and* by the object's transparency
    // value.
    //    The two components are then added together, and the sum is then multiplied
    // by the local surface color (which will tint both reflections and transmissions)
    // This is an approximation, and maybe we should eventually replace it with
    // something better
    Color localSurfColor = intersectedObject->GetSurfaceColor();
    surfaceColor = (reflectionColor*fresnelEffect +
      refractionColor*(1 - fresnelEffect) * intersectedObject->transparency);
    surfaceColor *= localSurfColor;
  }
  else {
    // it's a diffuse object, no need to raytrace any further; instead, trace some
    // shadow rays to lights
    bool blocked;
    Color lightIntensity(0);  // incoming spectrum from light
    Vec3f lightDirection;   // direction ray from light to p_hit
    float lightDistance;
    for (int il = 0; il < lights.size(); ++il) {
      int nSamplesForLight = lights[il]->NSamples();   // = 1, except for area lights
      float perSampleVisibilityFactor = 1.0 / nSamplesForLight;
      float visibility = 0.0;  // = 0--1 = complete shadowing to no shadowing from this light
      for (int nn = 0; nn < nSamplesForLight; nn++) {
        // get a new shadow ray toward light
        lights[il]->Illuminate(p_hit, lightDirection, lightIntensity, lightDistance);
        lightDirection.normalize();
//         if ((x == 7) && (y == 3))
//           printf("   p_hit = (%.2f,%.2f,%.2f), n_hit = (%.2f,%.2f,%.2f), lightDir = (%.2f,%.2f,%.2f), d = %f: ", 
//         		p_hit.x,p_hit.y,p_hit.z, n_hit.x,n_hit.y,n_hit.z, 
//         		lightDirection.x,lightDirection.y,lightDirection.z, lightDistance);
        blocked = TraceShadowRay(-lightDirection, lightDistance, objects, p_hit, n_hit);
//         if ((x == 7) && (y == 3))
//           printf(" blocked = %d\n", blocked);
        if (! blocked)
          visibility += perSampleVisibilityFactor;
      }
      if (visibility > 0.0) {
        // OK, light from this particular light source reaches this part of the object;
        // get the object's base diffuse + specular color combination
        Color objectBaseColor = intersectedObject->ComputeObjectColor(raydir, n_hit, lightDirection);
        surfaceColor += lightIntensity * visibility * objectBaseColor;
      }
    }
  }

  return surfaceColor + intersectedObject->GetEmissionColor();
}



// Main rendering function. We compute a camera ray for each pixel of the image,
// trace it, and return a color. 
// (If oversampleRate > 1, we do this multiple tiomes for each pixel and add up the
// weighted colors for each subsample).
// If a ray hits an object, we return the color of the object at the intersection 
// point, otherwise we return the background color.
void RenderImage( Scene *theScene, Color *image, const int width, const int height, 
				const traceOptions &options )
{
  Color *pixelArray = image;
  Camera *theCamera;
  Vec3f  cameraRay;
  int  oversampleRate = 1;
  float  xx, yy, oversampleScaling;
  float  t_newRay;  // will hold distance traveled by primary ray (not used, but needed by RayTrace)
  int  nSubsamples, nSoFar;
  int  nPixTot = width * height;
  int  tenPercent = (int)(nPixTot / 10);
  
  theCamera = theScene->GetCamera();
  theCamera->SetFOV(options.FieldOfView);
  theCamera->SetImageSize(width, height);

  // setup for oversampling
  theCamera->SetSampling(options.oversampling, options.samplerName);
  oversampleRate = options.oversampling;
  nSubsamples = oversampleRate*oversampleRate;
  oversampleScaling = 1.0 / (oversampleRate * oversampleRate);
  
  // Trace the rays, with possible per-pixel oversampling
  nSoFar = 0;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      Color cumulativeColor = Color(0);
      theCamera->UpdateSampler();
      for (int n = 0; n < nSubsamples; ++n) {
        cameraRay = theCamera->GenerateCameraRay(x, y, n, &xx, &yy);
//         if ((xx == 7) && (yy == 3))
//           printf("image x,y = %f,%f\n", xx,yy);
        cumulativeColor += RayTrace(Vec3f(0), cameraRay, theScene, 0, &t_newRay, xx, yy);
      }
      *pixelArray = cumulativeColor * oversampleScaling;
      ++pixelArray;
      nSoFar += 1;
      if ((nSoFar % tenPercent) == 0) {
      	int percentSoFar = (int)(nSoFar / tenPercent);
        printf("... %d0%% ", percentSoFar);
        fflush(stdout);
      }
    } 
  }
  
  printf("\nDone with render.\n");  
}
