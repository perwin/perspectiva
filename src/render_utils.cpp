#include <math.h>
#include <vector>

#include "geometry.h"
#include "render_utils.h"
#include "mersenne_twister.h"


// Generate random position (x,y) within unit disk, using rejection sampling
Vector UnitDisk_RejectionSample()
{
  float  x, y;
  do {
    // generate random numbers in range [-0.5,0.5]
    x = (float)genrand_real1() - 0.5;
    y = (float)genrand_real1() - 0.5;
  } while ((x*x + y*y) >= 0.25);
  // Generate point offset in x,y (multiply by 2 to convert [-0.5,0.5]
  // to [-1,1])
  return Vector(2.0*x, 2.0*y, 0);
}


Vector Refraction( const Vector &incident, const Vector &normal, float ior1, float ior2 )
{
  float  iorRatio, cosIncident, sin2Transmitted, cosTransmitted;
  
  iorRatio = ior1 / ior2;
  cosIncident = -Dot(normal, incident);
  sin2Transmitted = (iorRatio*iorRatio) * (1.0f - cosIncident*cosIncident);
  if (sin2Transmitted > 1.0) {
  	// total internal reflection
  	return Vector(-1e6);
  }
  cosTransmitted = sqrtf(1.0f - sin2Transmitted);
  return iorRatio*incident + (iorRatio*cosIncident - cosTransmitted)*normal;
}
