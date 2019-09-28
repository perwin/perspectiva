#include <math.h>
#include <vector>

#include "geometry.h"
#include "render_utils.h"
#include "mersenne_twister.h"


// Generate random position (x,y,0) within unit disk, using rejection sampling
// For simplicity in use by Camera::GenerateLensePoint, we actually output this
// as a Vector (in effect, the vector offset from the center of the unit circle).
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


/// Returns refraction direction as vector -- NO LONGER NEEDED?
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


/// Given an incident ray and the normal of the surface it's intersecting, along
/// with the inner and outer indices of refraction, this function computes the
/// mean Fresnel reflectivity R_eff and the direction vector for the transmitted
/// (refracted) outgoing ray; these are returned in the parameters R_eff and
/// refractionDir.
/// In the case of total internal reflection, then R_eff is set = 1 and
/// refractionDir is set = Vector(0).
void ComputeFresnelAndRefraction( const Vector &incidentRayDir, const Vector &normal, 
							float ior1, float ior2, float *R_eff, Vector &refractionDir )
{
  float  iorRatio, cosIncident, sin2Transmitted, cosTransmitted;

  iorRatio = ior1 / ior2;
  // we assume incident ray is in opposite direction to normal, so we want
  // -cos to get the correct incident angle
  cosIncident = -Dot(normal, incidentRayDir);
//   printf("incidentRayDir = (%f,%f,%f), normal = (%f,%f,%f): cosIncident = %f\n", incidentRayDir.x,
//   		incidentRayDir.y, incidentRayDir.z, normal.x, normal.y, normal.z, cosIncident);
  sin2Transmitted = (iorRatio*iorRatio) * (1.0f - cosIncident*cosIncident);
  if (sin2Transmitted > 1.0) {   // total internal reflection
  	*R_eff = 1.0;
  	refractionDir = Vector(0);
  }
  else {
    cosTransmitted = sqrtf(1.0f - sin2Transmitted);
//     printf("incidentRayDir = (%f,%f,%f): cosTransmitted = %f\n", incidentRayDir.x,
//   			incidentRayDir.y, incidentRayDir.z, cosTransmitted);
    float  n1_cosI = ior1*cosIncident;
    float  n1_cosT = ior1*cosTransmitted;
    float  n2_cosI = ior2*cosIncident;
    float  n2_cosT = ior2*cosTransmitted;
    float  R_s_sqrt = fabs((n1_cosI - n2_cosT) / (n1_cosI + n2_cosT));
    float  R_p_sqrt = fabs((n1_cosT - n2_cosI) / (n1_cosT + n2_cosI));
    *R_eff = 0.5*(R_s_sqrt*R_s_sqrt + R_p_sqrt*R_p_sqrt);
    refractionDir = iorRatio*incidentRayDir + (iorRatio*cosIncident - cosTransmitted)*normal;
    refractionDir = Normalize(refractionDir);
  }
}
