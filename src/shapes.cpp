#include <math.h>
#include <stdio.h>

#include "geometry.h"
#include "shapes.h"

// General notes for intersect methods:
//    rayorig is the point from which the ray starts [e.g., camera location, or point of 
//       most recent reflection/refraction]
//    raydir is the normalized ray direction vector
//    t0 = t-value for point of intersection along the ray (defined as rayorig + t*radir),
//       if it exists (*closest* point to rayorig if two intersections are possible)
//    t1 = optional point of second, more distant intersection; this only needs to
//       be computed by intersect() if such an intersection is possible [e.g., for a sphere]

// Get the point in 3D space defined by normalized direction vector raydir, starting
// from coordinate rayorig and extending for distance t
// <x, y, z> = <r_orig,x + dir_x * t, r_orig,y + dir_y * t, r_orig,z + dir_z * t>
Point GetIntersectionPoint( const Point &rayorig, const Vector &raydir, float t )
{
  return rayorig + t*raydir;
}


// bool Sphere::intersect( const Point &rayorig, const Vector &raydir, float *t0, float *t1 ) const
// {
//   Vector centerToRayOrig = center - rayorig; 
//   // compute distance along ray to impact parameter
//   //    full vector from ray origin to impact parameter = o + t_ca*raydir
//   //          = |centerToRayOrig| DOT raydir
//   float t_ca = Dot(centerToRayOrig, raydir);  // = |centerToRayOrig| cos(theta)
// //   printf("center = (%.1f,%.1f,%.1f), raydir = (%.1f,%.1f,%.1f): t_ca = %.2f\n", 
// //   		center.x, center.y, center.z, raydir.x, raydir.y, raydir.z, t_ca);
// //   printf("   centerToRayOrig = (%.1f,%.1f,%.1f)\n", centerToRayOrig.x, centerToRayOrig.y, centerToRayOrig.z);
//   if (t_ca < 0)  // is sphere entirely *behind* the camera?
//     return false;
//   // d = distance from sphere center to ray's closest approach to sphere center (impact parameter)
//   // Pythagorean Thm: d^2 + |centerToRayOrig|^2 = t_ca^2
//   float d2 = Dot(centerToRayOrig, centerToRayOrig) - t_ca*t_ca; 
// //  printf("   d2 = %f\n", d2);
//   if (d2 > radius2)  // does ray pass outside sphere?
//     return false; 
//   float t_hc = sqrt(radius2 - d2); 
//   *t0 = t_ca - t_hc; 
//   *t1 = t_ca + t_hc; 
// 
//   return true; 
// } 

// trial version using object coordinates
// sphere center is at (0,0,0) IF USE_TRANSFORMS is defined
bool Sphere::intersect( const Point &rayorig, const Vector &raydir, float *t0, float *t1 ) const
{
  Vector  centerToRayOrig;
  Point  rayorig_object;
  Vector  raydir_object;
  // Convert rayorig and raydir to Object coordinate system ...
#ifdef DEBUG
  //printf("      rayorig(input) = (%.2f,%.2f,%.2f)", rayorig.x,rayorig.y,rayorig.z);
#endif
  if (transformPresent) {
    rayorig_object = (*WorldToObject)(rayorig);
//     raydir_object = Normalize((*WorldToObject)(raydir));
    raydir_object = (*WorldToObject)(raydir);
  }
  else {
    rayorig_object = rayorig;
    raydir_object = raydir;
  }
  centerToRayOrig = center - rayorig_object;
  float t_ca = Dot(centerToRayOrig, raydir_object);
#ifdef DEBUG
//   printf(" -- rayorig_object(transformed) = (%.2f,%.2f,%.2f); t_ca = %.2f\n",
//   		rayorig_object.x,rayorig_object.y,rayorig_object.z, t_ca);
#endif
  if (t_ca < 0)  // is sphere entirely *behind* the camera/ray origin?
    return false; 
  float d2 = Dot(centerToRayOrig, centerToRayOrig) - t_ca*t_ca; 
  if (d2 > radius2)  // does ray pass outside sphere?
    return false; 
  // OK, if we're here, then ray intersected this sphere
  float t_hc = sqrt(radius2 - d2); 
  *t0 = t_ca - t_hc; 
  *t1 = t_ca + t_hc; 

  return true; 
} 


bool Plane::intersect( const Point &rayorig, const Vector &raydir, float *t0, float *t1 ) const
{
  float  denominator = Dot(norm, raydir);  // assumes that raydir is normalized
  if (fabs(denominator) > 1e-6) {
    *t0 = Dot(norm, center - rayorig) / denominator;
    // value of t1 is formally meaningless, but needs to be set in case caller
    // decides to check it
    *t1 = *t0;
    return (*t0 >= 0.0);
  }
  else
    return false;
}


// UNFINISHED!
// Equation for point on plane
// <x, y, z> = <r_orig,x + dir_x * t, r_orig,y + dir_y * t, r_orig,z + dir_z * t>
bool Rectangle::intersect( const Point &rayorig, const Vector &raydir, float *t0, float *t1 ) const
{
  float  denominator = Dot(norm, raydir);  // assumes that raydir is normalized
//  printf("   Plane: raydir = (%f,%f,%f), denominator = %f\n", raydir.x,raydir.y,raydir.z, denominator);
  if (fabs(denominator) > 1e-6) {
    *t0 = Dot(norm, center - rayorig) / denominator;
    if (*t0 >= 0.0) {
      // OK, we hit the plane the rectangle is in; now see if we're within rectangle
      Point intersection = GetIntersectionPoint(rayorig, raydir, *t0);
      float dx = fabs(intersection.x - center.x);
      float dy = fabs(intersection.x - center.y);
      float dz = fabs(intersection.x - center.z);
      // more stuff needed here!
    }
    else
      return false;
  }
  else
    return false;
}

