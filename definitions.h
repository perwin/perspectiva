// Definitions for raytracer

#include <string>

#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

// image formats
const int IMAGE_PPM = 0;
const int IMAGE_PNG = 1;
const int IMAGE_EXR = 2;

// names for samplers
const std::string SAMPLER_UNIFORM = "uniform";
const std::string SAMPLER_UNIFORM_JITTER = "uniform_jitter";

#endif  // _DEFINITIONS_H_
