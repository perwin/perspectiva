// Definitions for raytracer

#include <string>

#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

// light thypes
const int LIGHT_UNDEFINED = 0;
const int LIGHT_POINT = 1;
const int LIGHT_DISTANT = 2;

// image formats
const int IMAGE_PPM = 0;
const int IMAGE_PNG = 1;
const int IMAGE_EXR = 2;

// names for samplers
const std::string SAMPLER_UNIFORM = "uniform";
const std::string SAMPLER_UNIFORM_JITTER = "uniform_jitter";

#endif  // _DEFINITIONS_H_
