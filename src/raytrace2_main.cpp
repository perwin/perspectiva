// Based on code from scratchapixel.com

#include <cstdlib>
#include <cstdio> 
#include <cmath> 
#include <fstream> 
#include <vector> 
#include <iostream> 
#include <cassert> 
#include <stdio.h>
#include <time.h>
#include <sys/time.h>   // for timing-related functions and structs

#include "spdlog/spdlog.h"

#include "color.h"
#include "definitions.h"
#include "scene.h"
#include "utilities_pub.h"
#include "commandline_parser.h"
#include "option_structs.h"
#include "scenefile_parser.h"
#include "render.h"
#include "image_io.h"
#include "mersenne_twister.h"
 
 
const string LOG_FILENAME = "rt_log.txt";


/* Local Functions: */
void ProcessInput( int argc, char *argv[], commandOptions *theOptions );
 



// In the main function, we either load a user-supplied scene file, or create 
// a default scene.  Then, once the scene description is complete, 
// we render that scene (and save the resulting image) by calling the RenderAndSaveImage()
// function.
int main( int argc, char **argv )
{
  Scene  *theScene;
  struct timeval  timer_start, timer_end;
  double  microsecs, time_elapsed;
  commandOptions  options;
  traceOptions  raytraceOptions;
  
  init_genrand(time(NULL));

  auto logger = spdlog::basic_logger_mt("rt_logger", LOG_FILENAME.c_str());
  logger->info("");
  logger->info("Starting up main...");

  // Process command line 
  ProcessInput(argc, argv, &options);
  
  if (options.rngSeed > 0)
    init_genrand(options.rngSeed);
  else
    init_genrand((unsigned long)time(NULL));
  raytraceOptions.shadowTransparency = options.shadowTransparency;
  if (options.imageSizeSet) {
    raytraceOptions.width = options.imageWidth;
    raytraceOptions.height = options.imageHeight;
  }
  if (options.saveAlpha)
    raytraceOptions.mode = ALPHA_MASK;
  if (options.fieldOfViewSet) {
    // User wishes to override FOV specification in scene file, if any
    printf("\tField of view = %f\n", options.fieldOfView);
    raytraceOptions.fieldOfView = options.fieldOfView;
    raytraceOptions.fieldOfViewSet = true;
  }
  if (options.oversamplingRate > 0) {
    printf("\tOversampling pixels: %d x %d\n", options.oversamplingRate, options.oversamplingRate);
    raytraceOptions.oversampling = options.oversamplingRate;
  }
  if (options.samplerSet) {
    raytraceOptions.samplerName = options.samplerName;
    printf("\tUsing %s sampler\n", options.samplerName.c_str());
  }
  if (! options.noImageName) {
    size_t nChars = options.outputImageName.size();
    // look for output filename suffixes
    size_t found = options.outputImageName.find(".png", nChars - 4);
    if (found != std::string::npos)
      options.outputImageFormat = IMAGE_PNG;
    else {
      found = options.outputImageName.find(".exr", nChars - 4);
      if (found != std::string::npos)
        options.outputImageFormat = IMAGE_EXR;
      else
        options.outputImageFormat = IMAGE_PPM;
    }
  }
  
  if (options.noSceneFile) {
    theScene = new Scene();
    if (options.useTestScene) {
      theScene->AssembleTestScene();
      printf("\tNo scene file; using test scene...\n");
    }
    else {
      theScene->AssembleDefaultScene();
      printf("\tNo scene file; using default scene...\n");
    }
  }
  else {
    printf("\tReading scene from \"%s\"...\n", options.sceneFilename.c_str());
    theScene = LoadSceneFromFile(options.sceneFilename);
  }
  
  
  // Render scene and save image
  int w = raytraceOptions.width;
  int h = raytraceOptions.height;
  if ((w <= 0) || (h <= 0)) {
    printf("ERROR: requested image has bad dimensions! (w = %d pixels, h = %d pixels)\n",
    		w, h);
    return -1;
  }
  
  printf("Scene camera: FOV = %f\n", theScene->GetCamera()->fieldOfView);
  
  Color *image = new Color[w*h];
  printf("Starting render...\n");
  gettimeofday(&timer_start, NULL);
  RenderImage(theScene, image, w, h, raytraceOptions); 
  gettimeofday(&timer_end, NULL);
  microsecs = timer_end.tv_usec - timer_start.tv_usec;
  time_elapsed = timer_end.tv_sec - timer_start.tv_sec + microsecs/1e6;
  printf("Finished with render. (Elapsed time = %.6f sec)\n", time_elapsed);

  // Save image
  SaveImage(image, w, h, options.outputImageName, options.outputImageFormat);


  delete theScene;
  delete [] image;
  
  logger->info("*** Shutting down...");
  printf("\nInternal logging output saved to %s\n\n", LOG_FILENAME.c_str());
  spdlog::drop_all();

  return 0; 
}



void ProcessInput( int argc, char *argv[], commandOptions *theOptions )
{

  CLineParser *optParser = new CLineParser();
  string  tempString = "";

  /* SET THE USAGE/HELP   */
  optParser->AddUsageLine("Usage: ");
  optParser->AddUsageLine("   raytracer2 [options] [scene-file]");
  optParser->AddUsageLine(" -h  --help                         Prints this help");
  optParser->AddUsageLine(" --width <output-image width>       width of output image in pixels");
  optParser->AddUsageLine(" --height <output-image height>     height of output image in pixels");
  optParser->AddUsageLine(" -o  --output <output-image-root>   root name for output image [default = \"untitled\"]");
  optParser->AddUsageLine("                                    (add \".png\" to save in PNG format)");
  optParser->AddUsageLine("                                    (add \".exr\" to save in OpenEXR format)");
  optParser->AddUsageLine(" --FOV                              camera field of view (degrees; default = 30)");
  optParser->AddUsageLine(" --oversample                       pixel oversampling rate (must be positive integer)");
  optParser->AddUsageLine(" --sampler <sampler-name>           name of sampler to use [default = \"uniform\"]");
  optParser->AddUsageLine("                                       (\"uniform\", \"uniform_jitter\")");
  optParser->AddUsageLine(" --filter <filter-name>             name of image reconstruction filter to use [default = \"block\"]");
  optParser->AddUsageLine("                                       (\"block\", \"gaussian\") [NOT YET IMPLEMENTED!]");
  optParser->AddUsageLine(" --shadow-transparency              trace shadow rays through translucent objects");
  optParser->AddUsageLine(" --seed <rng-seed>                  integer for RNG seed (0 = use system time)");
  optParser->AddUsageLine(" --test-scene                       use internal test scene");
//  optParser->AddUsageLine(" --alpha                            specifies that output image should be alpha mask");
  optParser->AddUsageLine("");

  optParser->AddFlag("help", "h");
//  optParser->AddFlag("alpha");
  optParser->AddOption("output", "o");
  optParser->AddOption("width");
  optParser->AddOption("height");
  optParser->AddOption("oversample");
  optParser->AddOption("sampler");
  optParser->AddOption("filter");
  optParser->AddOption("FOV");
  optParser->AddOption("seed");
  optParser->AddFlag("shadow-transparency");
  optParser->AddFlag("test-scene");

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
  if (optParser->nArguments() > 0) {
    theOptions->sceneFilename = optParser->GetArgument(0);
    theOptions->noSceneFile = false;
  }

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
  if (optParser->OptionSet("oversample")) {
    if (NotANumber(optParser->GetTargetString("oversample").c_str(), 0, kPosInt)) {
      fprintf(stderr, "*** ERROR: oversample should be a positive integer!\n\n");
      delete optParser;
      exit(1);
    }
    int val = atol(optParser->GetTargetString("oversample").c_str());
    theOptions->oversamplingRate = val;
  }
  if (optParser->OptionSet("seed")) {
    if (NotANumber(optParser->GetTargetString("seed").c_str(), 0, kNonzeroInt)) {
      fprintf(stderr, "*** ERROR: seed should be a non-negative integer!\n\n");
      delete optParser;
      exit(1);
    }
    unsigned long val = atol(optParser->GetTargetString("seed").c_str());
    theOptions->rngSeed = val;
  }
  if (optParser->OptionSet("sampler")) {
    theOptions->samplerName = optParser->GetTargetString("sampler");
    theOptions->samplerSet = true;
  }
  if (optParser->OptionSet("filter")) {
    theOptions->filterName = optParser->GetTargetString("filter");
    theOptions->filterSet = true;
  }
  if (optParser->OptionSet("output")) {
    theOptions->outputImageName = optParser->GetTargetString("output");
    theOptions->noImageName = false;
  }
  if ( optParser->FlagSet("shadow-transparency") ) {
    theOptions->shadowTransparency = true;
    printf("Shadow rays will be traced through transparent objects!\n");
  }
  if ( optParser->FlagSet("test-scene") ) {
    theOptions->useTestScene = true;
    printf("Using test scene!\n");
  }

  delete optParser;

}


