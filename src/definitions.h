// Definitions for raytracer

#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include <cstdlib>
#include <string>

using namespace std;

// camera types (might never be more than just one...)
const int CAMERA_PERSPECTIVE = 0;


// light types
const int LIGHT_UNDEFINED = 0;
const int LIGHT_POINT = 1;
const int LIGHT_DISTANT = 2;
const int LIGHT_SPHERE = 10;
const int LIGHT_RECT = 11;


// image formats
const int IMAGE_PPM = 0;
const int IMAGE_PNG = 1;
const int IMAGE_EXR = 2;

// names for samplers
const string SAMPLER_UNIFORM = "uniform";
const string SAMPLER_UNIFORM_JITTER = "uniform_jitter";

// names for image reconstruction filters
const string FILTER_BLOCK = "block";
const string FILTER_GAUSSIAN = "gaussian";


// miscellaneous useful constants

const float PI = 3.141592653589793;
const float TWO_PI = 6.283185307179586;
const float FOUR_PI = 12.566370614359172;
const float PI_OVER_TWO = 1.5707963267948966;
const float DEG2RAD = PI / 180.0;
const float kInfinity = std::numeric_limits<float>::max(); 

const float BIAS = 1e-4;   // to make small offsets from object surfaces

const string NULL_MATERIAL_NAME = "**none**";

#endif  // _DEFINITIONS_H_
