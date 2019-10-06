// Code for functions used in render.cpp
// (Separate file mainly so we can do tests on them.)

#ifndef _RENDER_UTILS_H_
#define _RENDER_UTILS_H_

#include <string>
#include <vector>
#include "geometry.h"

/// Returns random position within unit disk
Vector UnitDisk_RejectionSample();

/// Returns random position within unit polygon
Vector UnitPolygon_RejectionSample( int nSides, const float vertices[] );

/// Returns refraction direction as vector -- NO LONGER NEEDED?
Vector Refraction( const Vector &incident, const Vector &normal, float ior1, float ior2 );

/// Computes Fresnel reflectance ratio R_eff and outgoing refraction vector
void ComputeFresnelAndRefraction( const Vector &incidentRayDir, const Vector &normal, 
							float ior1, float ior2, float *R_eff, Vector &refractionDir );


#endif   // _RENDER_UTILS_H_
