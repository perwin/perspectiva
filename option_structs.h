/*! \file
    \brief Structs containing different sets of options info for raytracer2.
 *
 */

#ifndef _OPTION_STRUCTS_H_
#define _OPTION_STRUCTS_H_

#include <string>
#include <vector>
#include "definitions.h"

const string DEFAULT_OUTPUT_IMAGE_FILENAME = "untitled";

/// struct for holding various general options (set by command-line flags & options)
/// Note that we initialize things inside the definition, which requires C++11
typedef struct {
  bool  noImageName = true;
  bool  imageSizeSet = false;
  bool  saveAlpha = false;
  unsigned  imageWidth = 0;
  unsigned  imageHeight = 0;
  float  fieldOfView = 0;
  bool  fieldOfViewSet = false;
  int  oversamplingRate = 0;
  std::string  outputImageName = DEFAULT_OUTPUT_IMAGE_FILENAME;
  int  outputImageFormat = IMAGE_PPM;
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
  unsigned width = 800;
  unsigned height = 600;
  float FieldOfView = 30.0;   // camera field of view in degrees
} traceOptions;



#endif  // _OPTION_STRUCTS_H_
