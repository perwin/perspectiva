// Code for functions used in render.cpp
// (Separate file mainly so we can do tests on them.)

#ifndef _RENDER_UTILS_H_
#define _RENDER_UTILS_H_

#include <string>
#include <vector>
#include "geometry.h"

Vector UnitDisk_RejectionSample();

Vector Refraction( const Vector &incident, const Vector &normal, float ior1, float ior2 );


#endif   // _RENDER_UTILS_H_
