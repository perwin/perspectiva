#include <cstdlib> 
#include <cstdio> 
#include <cmath> 
#include <fstream> 
#include <vector> 
#include <iostream> 
#include <cassert> 
#include <stdio.h>
#include <sys/time.h>   // for timing-related functions and structs

#include "vec3.h"
#include "geometry.h"
#include "scene.h"
#include "commandline_parser.h"
 
#if defined __linux__ || defined __APPLE__ 
// "Compiled for Linux
#else 
// Windows doesn't define these values by default, Linux does
#define M_PI 3.141592653589793 
#define INFINITY 1e8 
#endif 
 
 


// This variable controls the maximum recursion depth
#define MAX_RAY_DEPTH 5 
 
float mix( const float &a, const float &b, const float &mix ) 
{ 
  return b * mix + a * (1 - mix); 
} 


// This is the main trace function. It takes a ray as argument (defined by
// its origin and direction). We test if this ray intersects any of the
// geometry in the scene. If the ray intersects an object, we compute the
// intersection point, the normal at the intersection point, and shade this
// point using this information. Shading depends on the surface property
// (is it transparent, reflective, diffuse). The function returns a color
// for the ray. If the ray intersects an object that is the color of the
// object at the intersection point, otherwise it returns the background
// color.

Vec3f trace( const Vec3f &rayorig, const Vec3f &raydir, const std::vector<Sphere> &spheres, 
    		const int &depth ) 
{ 
  //if (raydir.length() != 1) std::cerr << "Error " << raydir << std::endl;
  float tnear = INFINITY; 
  const Sphere* sphere = NULL; 
  // find intersection of this ray with the sphere in the scene
  for (unsigned i = 0; i < spheres.size(); ++i) { 
    float t0 = INFINITY, t1 = INFINITY; 
    if (spheres[i].intersect(rayorig, raydir, t0, t1)) { 
      if (t0 < 0) 
        t0 = t1; 
      if (t0 < tnear) { 
          tnear = t0; 
          sphere = &spheres[i]; 
      } 
    } 
  } 
  // if there's no intersection return black or background color
  if (!sphere) 
    return Vec3f(2); 
  Vec3f surfaceColor = 0; // color of the ray/surfaceof the object intersected by the ray 
  Vec3f phit = rayorig + raydir * tnear; // point of intersection 
  Vec3f nhit = phit - sphere->center; // normal at the intersection point 
  nhit.normalize(); // normalize normal direction 
  // If the normal and the view direction are not opposite to each other
  // reverse the normal direction. That also means we are inside the sphere so set
  // the inside bool to true. Finally reverse the sign of IdotN which we want
  // positive.
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
    Vec3f refldir = raydir - nhit * 2 * raydir.dotProduct(nhit); 
    refldir.normalize(); 
    Vec3f reflection = trace(phit + nhit * bias, refldir, spheres, depth + 1); 
    Vec3f refraction = 0; 
    // if the sphere is also transparent compute refraction ray (transmission)
    if (sphere->transparency) { 
      float ior = 1.1, eta = (inside) ? ior : 1 / ior; // are we inside or outside the surface? 
      float cosi = -nhit.dotProduct(raydir); 
      float k = 1 - eta * eta * (1 - cosi * cosi); 
      Vec3f refrdir = raydir * eta + nhit * (eta *  cosi - sqrt(k)); 
      refrdir.normalize(); 
      refraction = trace(phit - nhit * bias, refrdir, spheres, depth + 1); 
    } 
    // the result is a mix of reflection and refraction (if the sphere is transparent)
    surfaceColor = ( 
      reflection * fresneleffect + 
      refraction * (1 - fresneleffect) * sphere->transparency) * sphere->surfaceColor; 
  } 
  else { 
    // it's a diffuse object, no need to raytrace any further
    for (unsigned i = 0; i < spheres.size(); ++i) { 
      if (spheres[i].emissionColor.x > 0) { 
        // this is a light
        Vec3f transmission = 1; 
        Vec3f lightDirection = spheres[i].center - phit; 
        lightDirection.normalize(); 
        for (unsigned j = 0; j < spheres.size(); ++j) { 
          if (i != j) { 
            float t0, t1; 
            if (spheres[j].intersect(phit + nhit * bias, lightDirection, t0, t1)) { 
              transmission = 0; 
              break; 
            } 
          } 
        } 
        surfaceColor += sphere->surfaceColor * transmission * 
        std::max(float(0), nhit.dotProduct(lightDirection)) * spheres[i].emissionColor; 
      } 
    } 
  } 

  return surfaceColor + sphere->emissionColor; 
}



// Main rendering function. We compute a camera ray for each pixel of the
// image trace it and return a color. If the ray hits a sphere, we return
// the color of the sphere at the intersection point, else we return the
// background color.
//void render( const std::vector<Sphere> &spheres )
void render( Scene &theScene, unsigned width=640, unsigned height=480 )
{ 
  // "camera" setup
//  unsigned width = 640, height = 480; 
  Vec3f *image = new Vec3f[width * height];
  Vec3f *pixel = image; 
  float  invWidth = 1 / float(width);
  float  invHeight = 1 / float(height); 
  float  fov = 30;
  float  aspectRatio = width / float(height); 
  float  angle = tan(M_PI * 0.5 * fov / 180.);
  
  // Trace rays
  for (unsigned y = 0; y < height; ++y) { 
    for (unsigned x = 0; x < width; ++x) {
      ++pixel;
      float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectRatio; 
      float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle; 
      Vec3f raydir(xx, yy, -1); 
      raydir.normalize(); 
      *pixel = trace(Vec3f(0), raydir, theScene.spheres, 0); 
    } 
  }
  
  // NOTE (PE): We can conver the ppm image to e.g. png using "convert" (ImageMagick):
  //    $ convert untitled.ppm untitled.png
  // Save result to a PPM image (keep these flags if you compile under Windows)
  std::ofstream ofs("./untitled.ppm", std::ios::out | std::ios::binary); 
  ofs << "P6\n" << width << " " << height << "\n255\n"; 
  for (unsigned i = 0; i < width * height; ++i) {
    // PE: min(1, image[i].x) ensures that rgb values are always <= 1.0
    // these are then multiplied by 255 to get into the 0,255 range
    ofs << (unsigned char)(std::min(float(1), image[i].x) * 255) << 
           (unsigned char)(std::min(float(1), image[i].y) * 255) << 
           (unsigned char)(std::min(float(1), image[i].z) * 255); 
  } 
  ofs.close(); 
  delete [] image; 
}



// In the main function, we will create the scene which is composed of 5
// spheres and 1 light (which is also a sphere). Then, once the scene
// description is complete we render that scene, by calling the render()
// function.
int main( int argc, char **argv )
{
  Scene  theScene;
  struct timeval  timer_start, timer_end;
  double  microsecs, time_elapsed;

//  srand48(13);
  
  // Assemble scene
  theScene.AssembleDefaultScene();
  
  
  printf("Starting render...\n");
  // Generate the image (including convolution, if requested, repeatedly
  gettimeofday(&timer_start, NULL);

  render(theScene, 800, 600); 

  gettimeofday(&timer_end, NULL);
  microsecs = timer_end.tv_usec - timer_start.tv_usec;
  time_elapsed = timer_end.tv_sec - timer_start.tv_sec + microsecs/1e6;
  printf("Finished with render. (Elapsed time = %.6f sec\n", time_elapsed);

  return 0; 
}
