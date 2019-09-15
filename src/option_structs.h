/*! \file
    \brief Structs containing different sets of options info for raytracer2.
 *
 */

#ifndef _OPTION_STRUCTS_H_
#define _OPTION_STRUCTS_H_

#include <string>
#include <vector>
#include "definitions.h"

const string DEFAULT_SAMPLER_NAME = "uniform";
const string DEFAULT_FILTER_NAME = "block";
const string DEFAULT_OUTPUT_IMAGE_FILENAME = "untitled";

/// struct for holding various general options (set by command-line flags & options)
/// Note that we initialize things inside the definition, which requires C++11
typedef struct {
  bool  noSceneFile = true;
  std::string  sceneFilename = "";
  bool  noImageName = true;
  bool  imageSizeSet = false;
  bool  saveAlpha = false;
  unsigned  imageWidth = 0;
  unsigned  imageHeight = 0;
  float  fieldOfView = 0;
  bool  fieldOfViewSet = false;
  int  oversamplingRate = 0;
  std::string  samplerName = SAMPLER_UNIFORM;
  bool  samplerSet = false;
  std::string  filterName = FILTER_BLOCK;
  bool  filterSet = false;
  std::string  outputImageName = DEFAULT_OUTPUT_IMAGE_FILENAME;
  int  outputImageFormat = IMAGE_PPM;
  bool shadowTransparency = false;
  bool  useTestScene = false;
} commandOptions;



const int DEFAULT_TRACE_MODE = 1;
const int ALPHA_MASK = 2;

const int MAX_RAY_DEPTH = 5;


/// struct for holding various raytracing options
/// Note that we initialize things inside the definition, which requires C++11
typedef struct {
  int maxRayDepth = MAX_RAY_DEPTH;
  int mode = DEFAULT_TRACE_MODE;
  int oversampling = 1;
  std::string  samplerName = SAMPLER_UNIFORM;
  unsigned width = 800;
  unsigned height = 600;
  float FieldOfView = 30.0;   // camera field of view in degrees
  bool shadowTransparency = false;   // trace shadow rays through transparent objects?
} traceOptions;



#endif  // _OPTION_STRUCTS_H_
