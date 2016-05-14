/*! \file
    \brief Struct containing options info for Trace() function in raytracer2, 
           along with function for setting up default values within the struct.
 *
 */

#ifndef _TRACE_OPTION_STRUCT_H_
#define _TRACE_OPTION_STRUCT_H_

#include <string>
#include <vector>

const int DEFAULT_TRACE_MODE = 1;
const int ALPHA_MASK = 2;

const int MAX_RAY_DEPTH = 5;


/// struct for holding various raytracing options
/// Note that we initialize things inside the definition, which requires C++11
typedef struct {
  int maxRayDepth = MAX_RAY_DEPTH;;
  int mode = DEFAULT_TRACE_MODE;
  int oversampling = 1;
  unsigned width = 800;
  unsigned height = 600;
  float FieldOfView = 30.0;   // camera field of view in angles
} traceOptions;



#endif  // _TRACE_OPTION_STRUCT_H_
