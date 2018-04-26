#include <math.h>
#include <stdio.h>

#include "vec3.h"
#include "geometry.h"

// General notes for intersect methods:
//    rayorig is the point from which the ray starts [e.g., camera location, or point of 
//       most recent reflection/refraction]
//    raydir is the ray direction vector
//    t0 = t-value for point of intersection along the ray (defined as rayorig + t*radir),
//       if it exists (*closest* point to rayorig if two intersections are possible)
//    t1 = optional point of second, more distant intersection; this only needs to
//       be set by the intersect method if such an intersection is possible [e.g., for a sphere]

bool Sphere::intersect( const Vec3f &rayorig, const Vec3f &raydir, float *t0, float *t1 ) const
{
  // d = distance from sphere center to ray's closest approach to sphere center (impact parameter)
  // 
  Vec3f centerToRayOrig = center - rayorig; 
  float t_ca = centerToRayOrig.dotProduct(raydir);
//   printf("center = (%.1f,%.1f,%.1f), raydir = (%.1f,%.1f,%.1f): t_ca = %.2f\n", 
//   		center.x, center.y, center.z, raydir.x, raydir.y, raydir.z, t_ca);
//   printf("   centerToRayOrig = (%.1f,%.1f,%.1f)\n", centerToRayOrig.x, centerToRayOrig.y, centerToRayOrig.z);
  if (t_ca < 0)  // is sphere entirely *behind* the camera?
    return false; 
  float d2 = centerToRayOrig.dotProduct(centerToRayOrig) - t_ca*t_ca; 
//  printf("   d2 = %f\n", d2);
  if (d2 > radius2)  // does ray pass outside sphere?
    return false; 
  float t_hc = sqrt(radius2 - d2); 
  *t0 = t_ca - t_hc; 
  *t1 = t_ca + t_hc; 

  return true; 
} 


bool Plane::intersect( const Vec3f &rayorig, const Vec3f &raydir, float *t0, float *t1 ) const
{
  float  denominator = norm.dotProduct(raydir);  // assumes that raydir is normalized
//  printf("   Plane: raydir = (%f,%f,%f), denominator = %f\n", raydir.x,raydir.y,raydir.z, denominator);
  if (fabs(denominator) > 1e-6) {
    *t0 = norm.dotProduct(center - rayorig) / denominator;
//    printf("      t0 = %f\n", *t0);
    return (*t0 >= 0.0);
  }
  else
    return false;
}

