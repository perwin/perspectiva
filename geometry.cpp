#include <math.h>

#include "vec3.h"
#include "geometry.h"


bool Sphere::intersect( const Vec3f &rayorig, const Vec3f &raydir, float *t0, float *t1 ) const
{ 
  Vec3f l = center - rayorig; 
  float tca = l.dotProduct(raydir); 
  if (tca < 0)  // is sphere *behind* the camera?
    return false; 
  float d2 = l.dotProduct(l) - tca * tca; 
  if (d2 > radius2)  // does ray pass outside sphere?
    return false; 
  float thc = sqrt(radius2 - d2); 
  *t0 = tca - thc; 
  *t1 = tca + thc; 

  return true; 
} 
