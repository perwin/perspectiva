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
#include "scenefile_parser.h"
#include "render.h"
 
 


/* Local Functions: */
void ProcessInput( int argc, char *argv[], commandOptions *theOptions );
 


// This constant controls the maximum recursion depth
//const int MAX_RAY_DEPTH = 5;




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
  optParser->AddUsageLine(" --FOV                camera field of view (degrees; default = 30)");
  optParser->AddUsageLine(" --alpha                specifies that output image should be alpha mask");
  optParser->AddUsageLine("");

  optParser->AddFlag("help", "h");
  optParser->AddFlag("alpha");
  optParser->AddOption("output", "o");
  optParser->AddOption("width");
  optParser->AddOption("height");
  optParser->AddOption("FOV");

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
  if (optParser->OptionSet("FOV")) {
    if (NotANumber(optParser->GetTargetString("FOV").c_str(), 0, kPosReal)) {
      fprintf(stderr, "*** ERROR: Field of view should be a positive number!\n\n");
      delete optParser;
      exit(1);
    }
    theOptions->fieldOfView = (float)atof(optParser->GetTargetString("FOV").c_str());
    theOptions->fieldOfViewSet = true;
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
  if (options.fieldOfViewSet) {
    printf("field of view = %f\n", options.fieldOfView);
    raytraceOptions.FieldOfView = options.fieldOfView;
  }
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


