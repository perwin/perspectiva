// Code for raytracing images

#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

#include "spdlog/spdlog.h"

#include "definitions.h"
#include "geometry.h"
#include "lights.h"
#include "scene.h"
#include "option_structs.h"
#include "render.h"
#include "cameras.h"
#include "environment_map.h"
#include "render_utils.h"



// Used for computing Fresnel effects in RayTrace()
float mix( const float a, const float b, const float mix )
{
  return b*mix + a*(1.0 - mix);
}


// Given a point p_hit with surface normal n_hit and vector lightDirection to some 
// light at a distance of lightDistance along the vector, determine whether any shapes 
// are in between the point and the light.
bool TraceShadowRay( const Vector &lightDirection, const float lightDistance, 
					const std::vector<shared_ptr<Shape>> shapes, const Point &p_hit,
					const Vector &n_hit, bool verbose )
{
  bool blocked = false;
  // Check to see if another shape is blocking path to light (shadow rays).
  // We use the same basic algorithm as for camera raytracing, which means that
  // lightDirection must be the direction ray from the shaded point *to*
  // the light. (So in most cases this function should be called with -lightDir
  // where lightDir is direction ray from light to point produced by a light's
  // Illuminate method.
  // Also note that we assume blocking shapes (between this point and the light)
  // are *opaque*; we are not attempting to handle transparent/translucent shapes
  // (which, to be correct, would involve refraction and caustics...)
  for (int j = 0; j < shapes.size(); ++j) {
    float t_0, t_1;
    if (shapes[j]->intersect(p_hit + n_hit*BIAS, lightDirection, &t_0, &t_1)) {
      // we intersected a shape; check to see if it's closer to us than the light
      if (verbose)
        printf("\n      TraceShadowRay -- intersection: shape j = %d, t_0,t_1 = %f,%f\n",
        			j, t_0, t_1);
      if ((t_0 < lightDistance) || (t_1 < lightDistance)) {
        blocked = true;
        break;
      }
    }
  }
  return blocked;
}

// Same as TraceShadowRay(), except we check to see if intersected objects
// are transparent (while not worrying about refraction).
bool TraceShadowRay2( const Vector &lightDirection, const float lightDistance, 
					const std::vector<shared_ptr<Shape>> shapes, const Point &p_hit,
					const Vector &n_hit, float *attenuation, bool verbose )
{
  // Check to see if another shape is blocking path to light (shadow rays).
  // We use the same basic algorithm as for camera raytracing, which means that
  // lightDirection must be the direction ray from the shaded point *to*
  // the light. (So in most cases this function should be called with -lightDir
  // where lightDir is direction ray from light to point produced by a light's
  // Illuminate method.
  for (int j = 0; j < shapes.size(); ++j) {
    float t_0, t_1;
    *attenuation = 1.0;
    if (shapes[j]->intersect(p_hit + n_hit*BIAS, lightDirection, &t_0, &t_1)) {
      // we intersected a shape; check to see if it's closer to us than the light
      if (verbose)
        printf("\n      TraceShadowRay -- intersection: shape j = %d, t_0,t_1 = %f,%f\n",
        			j, t_0, t_1);
      if ((t_0 < lightDistance) || (t_1 < lightDistance)) {
        // OK, check if it's at least partially transparent
        shared_ptr<Material> thisMaterial = shapes[j]->GetMaterial();
        if (thisMaterial->translucent) {
          // FIXME: handle partial transparency properly!
          ;
          // *attenuation *= shapes[j]->transparency;  -- WRONG!
        } 
        else  // opaque object between use and light --> BLOCKED!
    	  return true;
      }
    }
  }
  // if we reached here, then there were no *opaque* shapes between us and the light,
  // so we're NOT blocked
  return false;
}


// This is the main ray tracing function. It takes a ray as argument (defined by its origin
// and direction). We test if this ray intersects any of the geometry in the scene. 
// If the ray intersects a shape, we compute the intersection point and the normal at
// the intersection point, and then shade this point using this information. Shading 
// depends on the surface property (is it transparent, reflective, diffuse). The function 
// returns a color for the ray. If the ray intersects a shape, this is the color of the 
// shape at the intersection point, otherwise it returns the background color.
//    x,y = pixel coordinates for debugging printouts
Color RayTrace( const Ray currentRay, shared_ptr<Scene> theScene, float *t, const float x=0.f, 
				const float y=0.f, bool transparentShadows=false, bool debug=false )
{
  std::vector<shared_ptr<Shape>> shapes = theScene->shapes;
  std::vector<Light *> lights = theScene->lights;
  shared_ptr<Environment> environment = theScene->environment;
  float  t_newRay;  // will hold distance to intersection of any reflection or
                    // transmission rays launched by this function
  auto logger = spdlog::get("rt_logger");
  if (debug)
    logger->debug("   Starting RayTrace...");

  // extract Ray data for convenience
  Point  rayorig = currentRay.o;
  Vector  raydir = currentRay.dir;
  int  depth = currentRay.depth;
  
  float t_nearest = kInfinity;
  shared_ptr<Shape> intersectedShape = NULL;
  // find intersection of this ray with shapes in the scene
  int  intersectedObjIndex = -1;
  for (int i = 0; i < shapes.size(); ++i) {
    float t_0 = kInfinity;
    float t_1 = kInfinity;
    if (shapes[i]->intersect(rayorig, raydir, &t_0, &t_1)) {
      if (t_0 < 0)  // first intersection is *behind* camera, so use the second
        t_0 = t_1;
      if (t_0 < t_nearest) {
          // this shape is closest of all so far
          t_nearest = t_0;
          intersectedShape = shapes[i];
          intersectedObjIndex = i;
      }
    }
  }
  *t = t_nearest;
  if (debug) {
    logger->debug("   RayTrace: End of intersection search: t_nearest = {:f}", t_nearest);
    if (! intersectedShape)
      logger->debug("             No intersection found!");
  }
  // if there's no intersection, return background color
  if (! intersectedShape)
    return environment->GetEnvironmentColor(currentRay);
  
  
  shared_ptr<Material> material = intersectedShape->GetMaterial();
  Color surfaceColor = 0;   // color of the surface of the shape intersected by the ray
  Point p_hit = rayorig + raydir*t_nearest;   // intersection point
  Vector n_hit = intersectedShape->GetNormalAtPoint(p_hit);   // normal at intersection point
  n_hit = Normalize(n_hit);   // normalize normal direction
  if (debug) {
    logger->debug("   RayTrace: p_hit = ({:.2f},{:.2f},{:.2f})", p_hit.x,p_hit.y,p_hit.z);
    logger->debug("   RayTrace: n_hit = ({:.2f},{:.2f},{:.2f})", n_hit.x,n_hit.y,n_hit.z);
  }
  // If the normal and the view direction are NOT opposite to each other, reverse 
  // the normal direction. That also means we are e.g. inside a sphere, so set the inside 
  // bool to true.
  bool inside = false;
  if (Dot(raydir, n_hit) > 0) {
    n_hit = -n_hit;
    inside = true;
  }
  if (debug)
    logger->debug("   RayTrace: n_hit = ({:.2f},{:.2f},{:.2f})", n_hit.x,n_hit.y,n_hit.z);

  
  if (debug)
    logger->debug("   RayTrace: metallic = {}, specular = {}, translucent = {}", 
    				material->metallic, material->specular, material->translucent);
  // Handle reflection and refraction, if material allows it
  if ( (material->metallic || material->specular || material->translucent) 
  		&& depth < MAX_RAY_DEPTH ) {
  	// Fresnel reflectance -- default to 1.0 in case material is opaque (in which case
  	// we skip proper Fresnel calculation); this allows for reflective opaque
  	// materials (e.g., metals)
  	float  R_fresnel = 1.0;
  	
  	// Reflection
    Color cumulativeReflectionColor = 0;
    // if the shape is reflective, compute reflection ray
    if ( material->metallic || material->specular ) {
      Vector refldir = raydir - n_hit*2*Dot(raydir, n_hit);  // reflection direction
      // This is a reflection, so IOR doesn't change
      Ray reflectionRay(p_hit + n_hit*BIAS, refldir, depth + 1, currentRay.currentIOR);
      if (debug) {
        logger->debug("      *Launching reflection ray...");
        logger->debug("      raydir = ({:f},{:f},{:f})", refldir.x,refldir.y,refldir.z);
      }
      cumulativeReflectionColor = RayTrace(reflectionRay, theScene, &t_newRay, 0.0,0.0,
      										transparentShadows);
      if (debug)
        logger->debug("      RETURNED: t_newRay = {:f}; color = ({:f},{:f},{:f})",
        			t_newRay, cumulativeReflectionColor.r,cumulativeReflectionColor.g,
        			cumulativeReflectionColor.b);
      cumulativeReflectionColor *= material->GetReflectionColor();
      if (debug)
        logger->debug("      color = ({:f},{:f},{:f})", cumulativeReflectionColor.r,
        				cumulativeReflectionColor.g, cumulativeReflectionColor.b);
    }
    
    // Refraction
    Color cumulativeRefractionColor = 0;
    // if the shape is transparent, compute proper Fresnel terms and 
    // transmission/refraction ray
    if (material->translucent) {
      // Compute Fresnel effect and refraction
      float  outgoingIOR;
      // FIXME: the following is a bit of a kludge, since it only works if we
      // have isolated refractive object in the scene (i.e., what if we have
      // one refractive object inside another?)
      if (inside)  // this is a bit of a kludge
        outgoingIOR = DEFAULT_IOR;
      else
        outgoingIOR = material->IOR;   
      Vector  refractionDir;
      ComputeFresnelAndRefraction(raydir, n_hit, currentRay.currentIOR, outgoingIOR,
      								&R_fresnel, refractionDir);

      //refractionDir = Refraction(raydir, n_hit, currentRay.currentIOR, outgoingIOR);
      if (debug) {
        logger->debug("      *Launching refraction ray...");
        logger->debug("      raydir = ({:f},{:f},{:f}); outgoinIOR = {:f}", 
        				refractionDir.x,refractionDir.y,refractionDir.z, outgoingIOR);
      }
      Ray refractionRay(p_hit - n_hit*BIAS, refractionDir, depth + 1, outgoingIOR);
      cumulativeRefractionColor = RayTrace(refractionRay, theScene, &t_newRay, 0.0,0.0,transparentShadows);
      if (debug)
        logger->debug("      RETURNED: t_newRay = {:f}; color = ({:f},{:f},{:f})",
        			t_newRay, cumulativeRefractionColor.r,cumulativeRefractionColor.g,
        			cumulativeRefractionColor.b);
      cumulativeRefractionColor *= material->GetRefractionColor();
      // possible application of Beer's Law:
      //    if shape material includes attenuation of transmitted light:
      //      if refraction ray goes *into* the surface, get distance t = t_newRay traveled by
      //      ray, then multiply (L_r,L_g,L_b) by exp(-k_r*t), exp(-k_g*t), exp(-k_b*t)
    }
    
    // the final result is a mix of contributions from reflection and refraction
    // FIXME:
    //    Right now, we're using a modified version of the Scratchapixel "very basic 
    // raytracer" approach, in which the reflection is unmodulated by any surface 
    // reflectivity value (though it is multiplied by the Fresnel reflectance), while 
    // the refraction is multiplied by both the Fresnel (1 - K_r) *and* by the shape's 
    // transparency value.
    //    The two components are then added together, and the sum is then multiplied
    // by the local surface color (which will tint both reflections and transmissions)
    // This is an approximation, and maybe we should eventually replace it with
    // something better
    
//     surfaceColor = R_fresnel*cumulativeReflectionColor +
//     					(1.0 - R_fresnel)*cumulativeRefractionColor*intersectedShape->transparency;
    surfaceColor = R_fresnel*cumulativeReflectionColor +
    					(1.0 - R_fresnel)*cumulativeRefractionColor;
  }
  
  
  else {
    // it's a diffuse shape, no need to raytrace any further; instead, trace some
    // shadow rays to lights
    bool blocked;
    bool verboseShadowRay = false;
    if (debug)
      logger->debug("      *Diffuse reflection:");
    Color lightIntensity(0);   // incoming spectrum from light
    Vector lightDirection;   // direction ray *from light to p_hit*
    float lightDistance;
    for (int i_light = 0; i_light < lights.size(); ++i_light) {
      int nSamplesForLight = lights[i_light]->NSamples();   // = 1, except for area lights
      float perSampleVisibilityFactor = 1.0 / nSamplesForLight;
      float visibility = 0.0;  // = 0--1 = complete shadowing to no shadowing from this light
      for (int nn = 0; nn < nSamplesForLight; nn++) {
        // get a new shadow ray toward light
        lights[i_light]->Illuminate(p_hit, lightDirection, lightIntensity, lightDistance);
        lightDirection = Normalize(lightDirection);
        if (debug) {
          verboseShadowRay = true;
          logger->debug("      Tracing shadow ray: lightDirection = ({:.2f},{:.2f},{:.2f}), d = {:f}", 
		        		lightDirection.x,lightDirection.y,lightDirection.z, lightDistance);
//           logger->debug("      p_hit = ({:.2f},{:.2f},{:.2f}), n_hit = ({:.2f},{:.2f},{:.2f})",
//           				p_hit.x,p_hit.y,p_hit.z, n_hit.x,n_hit.y,n_hit.z);
//           logger->debug("      lightDirection = ({:.2f},{:.2f},{:.2f}), d = {:f}", 
// 		        		lightDirection.x,lightDirection.y,lightDirection.z, lightDistance);
        }
		float translucencyFactor = 1.0;
        // do we trace shadow rays through transparent/translucent objects?
		if (transparentShadows)
          blocked = TraceShadowRay2(-lightDirection, lightDistance, shapes, p_hit, n_hit, 
    	      						&translucencyFactor, verboseShadowRay);
    	else
          blocked = TraceShadowRay(-lightDirection, lightDistance, shapes, p_hit, n_hit, 
        							verboseShadowRay);
        if (debug)
          logger->debug("      blocked = {}", blocked);
        if (! blocked)
          visibility += translucencyFactor*perSampleVisibilityFactor;
      }
      if (visibility > 0.0) {
        // OK, light from this particular light source reaches this part of the shape;
        // get the shape's base diffuse + specular color combination
        Color shapeBaseColor = material->GetDiffuseColor(raydir, n_hit, lightDirection);
        surfaceColor += lightIntensity * visibility * shapeBaseColor;
      }
    }
  }

  if (debug) {
    logger->debug("   Finishing RayTrace: surfaceColor = ({:f}, {:f}, {:f})", 
  				surfaceColor.r, surfaceColor.g, surfaceColor.b);
    Color emiss = material->GetEmissionColor();
  	logger->debug("      emissionColor = ({:f}, {:f}, {:f})", emiss.r, emiss.g, emiss.b);
  }
  return surfaceColor + material->GetEmissionColor();
}



// Main rendering function. We compute a camera ray for each pixel of the image,
// trace it, and return a color. 
// (If oversampleRate > 1, we do this multiple tiomes for each pixel and add up the
// weighted colors for each subsample).
// If a ray hits a shape, we return the color of the shape at the intersection 
// point, otherwise we return the background color.
void RenderImage( shared_ptr<Scene> theScene, Color *image, const int width, const int height, 
				const traceOptions &options )
{
  Color *pixelArray = image;
  std::shared_ptr<Camera> theCamera;
  //Ray  cameraRay;
  Vector  cameraRay_dir;
  int  oversampleRate = 1;
  float  xx, yy, oversampleScaling;
  float  t_newRay;  // will hold distance traveled by primary ray (not used, but needed by RayTrace)
  int  nSubsamples, iCurrentPix;
  int  nPixTot = width * height;
  int  tenPercent = (int)(nPixTot / 10);
  
  auto logger = spdlog::get("rt_logger");
  logger->info("Starting RenderImage...");
  
  theCamera = theScene->GetCamera();
  if (options.fieldOfViewSet)
    theCamera->SetFOV(options.fieldOfView);
  theCamera->SetImageSize(width, height);

  // setup for oversampling
  theCamera->SetSampling(options.oversampling, options.samplerName);
  oversampleRate = options.oversampling;
  nSubsamples = oversampleRate*oversampleRate;
  oversampleScaling = 1.0 / (oversampleRate * oversampleRate);
  
  
  // SPECIAL SINGLE-PIXEL DEBUGGING MODE
  if (options.singlePixelMode) {
    logger->debug("RenderImage: Single-pixel mode!");
    Color cumulativeColor = Color(0);
    int x = options.singlePixel_x;
    int y = options.singlePixel_y;
    Ray cameraRay = theCamera->GenerateCameraRay(x, y, 0, &xx, &yy);
    cumulativeColor += RayTrace(cameraRay, theScene, &t_newRay, xx, yy,
        						options.shadowTransparency, true);
    logger->debug("RenderImage: Done with single-pixel debug mode.");
    return;
  }
  
  
  // NORMAL FULL-IMAGE-RENDERING MODE
  // Trace the rays, with possible per-pixel oversampling
  int nDone = 0;

#pragma omp parallel private(iCurrentPix,xx,yy,t_newRay)
  {
  // specify static scheduling with fixed chunk size to keep things deterministic
  #pragma omp for schedule(static,10)
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      Color cumulativeColor = Color(0);
      //theCamera->UpdateSampler();
      for (int n = 0; n < nSubsamples; ++n) {
        Ray cameraRay = theCamera->GenerateCameraRay(x, y, n, &xx, &yy);
        if (theCamera->apertureRadius > 0.0) {
          // Depth-of-field!
          // Determine intersection of cameraRay with focalDistance plane
          // (cameraRay.dir.z is < 0, so we need to take negative of that to
          // get positive distance value)
          float  dist_to_focalPlane = theCamera->focalDistance / (-cameraRay.dir.z);
          Point focalPoint = cameraRay(dist_to_focalPlane);
          // Pick point on camera "lens"
          Point lensOffsetPoint = theCamera->GenerateLensPoint();
          cameraRay = Ray(lensOffsetPoint, focalPoint - lensOffsetPoint);
        }
        cumulativeColor += RayTrace(cameraRay, theScene, &t_newRay, xx, yy,
        							options.shadowTransparency);
      }
      iCurrentPix = y*width + x;
      pixelArray[iCurrentPix] = cumulativeColor * oversampleScaling;
      nDone++;
      if ((nDone % tenPercent) == 0) {
      	//int percentSoFar = (int)(nDone / tenPercent);
        printf("... %d0%% ", (int)(nDone / tenPercent));
        fflush(stdout);
      }
    } 
  }

  } // end omp parallel section

  logger->info("RenderImage: Done with render.");
  printf("\nDone with render.\n");  
}
