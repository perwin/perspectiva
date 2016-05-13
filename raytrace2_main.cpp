// Based on code from scratchapixel

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
#include "utilities_pub.h"
#include "commandline_parser.h"
#include "command_options.h"
#include "trace_options.h"
 
 
#if defined __linux__ || defined __APPLE__ 
// "Compiled for Linux
#else 
// Windows doesn't define these values by default, Linux does
#define M_PI 3.141592653589793 
#define INFINITY 1e8 
#endif 


/* Local Functions: */
void ProcessInput( int argc, char *argv[], commandOptions *theOptions );
float mix( const float &a, const float &b, const float &mix );
Vec3f Trace( const Vec3f &rayorig, const Vec3f &raydir, const std::vector<Sphere> &spheres, 
    		const int &depth );
void SaveImage( Vec3f *image, unsigned width=640, unsigned height=480, string imageFilename="untitled" );
void RenderAndSaveImage( Scene &theScene, unsigned width, unsigned height, 
			string filename, const traceOptions &options );
void ProcessInput( int argc, char *argv[], commandOptions *theOptions );

 


// This constant controls the maximum recursion depth
//const int MAX_RAY_DEPTH = 5;


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
  for (unsigned i = 0; i < objects.size(); ++i) { 
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
    for (unsigned i = 0; i < objects.size(); ++i) { 
      Vec3f emissColor = objects[i].emissionColor;
      if ((emissColor.x > 0) || (emissColor.y > 0) || (emissColor.z > 0) ) { 
        // this is a light
        Vec3f transmission = 1; 
        Vec3f lightDirection = objects[i].center - phit; 
        lightDirection.normalize(); 
        for (unsigned j = 0; j < objects.size(); ++j) { 
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
  
  std::ofstream ofs(outputFilename.c_str(), std::ios::out | std::ios::binary); 
  ofs << "P6\n" << width << " " << height << "\n255\n"; 
  for (unsigned i = 0; i < width*height; ++i) {
    // PE: min(1, image[i].x) ensures that rgb values are always <= 1.0
    // these are then multiplied by 255 to get into the 0,255 range
    ofs << (unsigned char)(std::min(float(1), image[i].x)*255) << 
           (unsigned char)(std::min(float(1), image[i].y)*255) << 
           (unsigned char)(std::min(float(1), image[i].z)*255); 
  } 
  ofs.close(); 
  
  printf("Saved image file \"%s\".\n", outputFilename.c_str());
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
  float  fov = 30;
  float  angle = tan(M_PI*0.5*fov / 180.);
  
  bool  doOversampling = false;
  float  oversampleRate = 1.0;
  float  xx0, yy0;
  
  if (options.overSampling > 1) {
    doOversampling = true;
    oversampleRate = options.overSampling;
  }
  
  // Trace the rays
  for (unsigned y = 0; y < height; ++y) {
    for (unsigned x = 0; x < width; ++x) {
      xx0 = (2*((x + 0.5)*invWidth) - 1.0) * angle*aspectRatio;
      yy0 = (1.0 - 2*((y + 0.5)*invHeight)) * angle;
      Vec3f raydir(xx0, yy0, -1.0); 
      raydir.normalize(); 
      pixelColor = Trace(Vec3f(0), raydir, theScene.objects, 0);
      
      *pixel = pixelColor;
      ++pixel;
    } 
  }
  
  printf("Done with render.\n");
  SaveImage(image, width, height, filename);

  delete [] image; 
}



// In the main function, we will create the scene which is composed of 5 spheres and 
// 1 light (which is also a sphere). Then, once the scene description is complete, 
// we render that scene (and save the resulting image) by calling the RenderAndSaveImage()
// function.
int main( int argc, char **argv )
{
  Scene  theScene;
  struct timeval  timer_start, timer_end;
  double  microsecs, time_elapsed;
  commandOptions  options;
  traceOptions  raytraceOptions;

  // Process command line 
  ProcessInput(argc, argv, &options);
  if (options.imageSizeSet) {
    raytraceOptions.width = options.imageWidth;
    raytraceOptions.height = options.imageHeight;
  }
  if (options.saveAlpha)
    raytraceOptions.mode = ALPHA_MASK;

  // Assemble scene
  theScene.AssembleDefaultScene();
  
  
  unsigned w = raytraceOptions.width;
  unsigned h = raytraceOptions.height;
  if ((w <= 0) || (h <= 0)) {
    printf("ERROR: requested image has bad dimensions! (w = %d pixels, h = %d pixels)\n",
    		w, h);
    return -1;
  }
  printf("Starting render...\n");
  gettimeofday(&timer_start, NULL);
  RenderAndSaveImage(theScene, w, h, options.outputImageName, raytraceOptions); 

  gettimeofday(&timer_end, NULL);
  microsecs = timer_end.tv_usec - timer_start.tv_usec;
  time_elapsed = timer_end.tv_sec - timer_start.tv_sec + microsecs/1e6;
  printf("Finished with render. (Elapsed time = %.6f sec)\n", time_elapsed);

  return 0; 
}



void ProcessInput( int argc, char *argv[], commandOptions *theOptions )
{

  CLineParser *optParser = new CLineParser();
  string  tempString = "";

  /* SET THE USAGE/HELP   */
  optParser->AddUsageLine("Usage: ");
  optParser->AddUsageLine("   raytracer2 [options]");
  optParser->AddUsageLine(" -h  --help                   Prints this help");
  optParser->AddUsageLine(" --width <output-image width>        width of output image in pixels");
  optParser->AddUsageLine(" --height <output-image height>        height of output image in pixels");
  optParser->AddUsageLine(" -o  --output <output-image-root>        root name for output image [default = untitled]");
  optParser->AddUsageLine(" --alpha                specifies that output image should be alpha mask");
  optParser->AddUsageLine("");

  optParser->AddFlag("help", "h");
  optParser->AddFlag("alpha");
  optParser->AddOption("output", "o");
  optParser->AddOption("width");
  optParser->AddOption("height");

  // Comment this out if you want unrecognized (e.g., mis-spelled) flags and options
  // to be ignored only, rather than causing program to exit
  optParser->UnrecognizedAreErrors();

  /* parse the command line:  */
  int status = optParser->ParseCommandLine( argc, argv );
  if (status < 0) {
    printf("\nError on command line... quitting...\n\n");
    delete optParser;
    exit(1);
  }


  /* Process the results: actual arguments, if any: */
//   if (optParser->nArguments() > 0) {
//     theOptions->configFileName = optParser->GetArgument(0);
//     theOptions->noConfigFile = false;
//   }

  /* Process the results: options */
  // First two are options which print useful info and then exit the program
  if ( optParser->FlagSet("help") ) {
    optParser->PrintUsage();
    delete optParser;
    exit(1);
  }
  if (optParser->OptionSet("width")) {
    if (NotANumber(optParser->GetTargetString("width").c_str(), 0, kPosInt)) {
      fprintf(stderr, "*** ERROR: width should be a positive integer!\n\n");
      delete optParser;
      exit(1);
    }
    theOptions->imageWidth = atol(optParser->GetTargetString("width").c_str());
    theOptions->imageSizeSet = true;
  }
  if (optParser->OptionSet("height")) {
    if (NotANumber(optParser->GetTargetString("height").c_str(), 0, kPosInt)) {
      fprintf(stderr, "*** ERROR: height should be a positive integer!\n\n");
      delete optParser;
      exit(1);
    }
    theOptions->imageHeight = atol(optParser->GetTargetString("height").c_str());
    theOptions->imageSizeSet = true;
  }
  if ( optParser->FlagSet("alpha") ) {
    theOptions->saveAlpha = true;
    printf("Alpha!\n");
  }
  if (optParser->OptionSet("output")) {
    theOptions->outputImageName = optParser->GetTargetString("output");
    theOptions->noImageName = false;
  }

  delete optParser;

}

