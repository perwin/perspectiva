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



const int LARGENUM = 100000;


// Given three collinear points p, q, r, the function checks if 
// point q lies on line segment 'pr' 
bool IsOnSegment( float px, float py, float qx, float qy, float rx, float ry )
{
  if ( (qx <= fmaxf(px, rx)) && (qx >= fminf(px, rx)) && 
       (qy <= fmaxf(py, ry)) && (qy >= fminf(py, ry)) )
    return true;
  else
    return false;
}


// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are collinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int Orientation( float px, float py, float qx, float qy, float rx, float ry )
{
  float val = (qy - py)*(rx - qx) - (qx - px)*(ry - qy);

  if (val == 0)
    return 0;    // collinear
  if (val > 0)
    return 1;   // clockwise
  return 2;     // counter-clockwise
}


// Returns true if line segments 'p1q1' and 'p2q2' intersect. 
bool SegmentIntersectionTest( float p1x, float p1y, float q1x, float q1y, 
							float p2x, float p2y, float q2x, float q2y )
{
  // Find orientations needed for general and special cases
  int  o1, o2, o3, o4;
  o1 = Orientation(p1x,p1y, q1x,q1y, p2x,p2y);
  o2 = Orientation(p1x,p1y, q1x,q1y, q2x,q2y);
  o3 = Orientation(p2x,p2y, q2x,q2y, p1x,p1y);
  o4 = Orientation(p2x,p2y, q2x,q2y, q1x,q1y);

  // General case
  if ((o1 != o2) && (o3 != o4))
    return true;

  // Special Cases 
  // p1, q1 and p2 are collinear and p2 lies on segment p1-q1 
  if (o1 == 0 && IsOnSegment(p1x,p1y, p2x,p2y, q1x,q1y))
    return true;

  // p1, q1 and p2 are collinear and q2 lies on segment p1-q1 
  if (o2 == 0 && IsOnSegment(p1x,p1y, q2x,q2y, q1x,q1y))
    return true;
  
  // p2, q2 and p1x,p1y are collinear and p1 lies on segment p2-q2 
  if (o3 == 0 && IsOnSegment(p2x,p2y, p1x,p1y, q2x,q2y))
    return true;

  // p2, q2 and q1 are collinear and q1 lies on segment p2-q2 
  if (o4 == 0 && IsOnSegment(p2x,p2y, q1x,q1y, q2x,q2y))
    return true;
      
  return false;     // Doesn't fall in any of the above cases 
}


bool InsidePolygon( float x, float y, int nSides, const float vertices[] )
{
  if (nSides < 3)
    return false;

  int count = 0;
  int i = 0;
  do {
    float  ix, iy, nextx, nexty;
    int next = (i + 1) % nSides;
    // get current vertex and next vertex
    ix = vertices[2*i];
    iy = vertices[2*i + 1];
    nextx = vertices[2*next];
    nexty = vertices[2*next + 1];
    if (SegmentIntersectionTest(ix,iy, nextx,nexty, x,y, LARGENUM,y)) {
      // if the point (x,y) is collinear with the polygon line segment connecting
      // points (ix,iy) and (nextx,nexty), then check to see if it lies *on* the
      // segment. Return true if it does, false otherwise.
      if (Orientation(ix,iy, x,y, nextx,nexty) == 0)
        return IsOnSegment(ix,iy, x,y, nextx,nexty);
      else
        count++;
    }
    i = next;
  } while (i != 0);
  
  // Return true if count is odd, false otherwise
  if ((count % 2) == 1)
    return true;
  else
    return false;
}


// Generate random position (x,y,0) within unit polygon, using rejection sampling
// For simplicity in use by Camera::GenerateLensePoint, we actually output this
// as a Vector (in effect, the vector offset from the center of the unit circle).
Vector UnitPolygon_RejectionSample( int nSides, const float vertices[] )
{
  float  x, y;
  do {
    // generate random numbers in range [-1,1]
    x = 2.0 * ((float)genrand_real1() - 0.5);
    y = 2.0 * ((float)genrand_real1() - 0.5);
  } while (! InsidePolygon(x, y, nSides, vertices));
  // Generate point offset in x,y
  return Vector(x, y, 0);
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
