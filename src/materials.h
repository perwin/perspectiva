// in-development header for material classes

#ifndef _MATERIALS_H_
#define _MATERIALS_H_

#include "geometry.h"
#include "color.h"
#include "render_utils.h"

const float DEFAULT_DIFFUSE_IOR = 1.5;
const float ONE_OVER_PI = 0.3183098861837907;


// metal: metallic = true
// general dielectric: metallic = false, specular = true, translucent = true
// opaque dielectric: metallic = false, specular = true, translucent = false
// opaque, diffuse dielectric: metallic = false, specular = false, translucent = false

class Material
{
  public:
    Material( ) // set default values: simple diffuse
    {
      baseColor = Color(1);
      metallic = false;
      roughness = 0.0;
      specular = false;
      translucent = false;
      refractionColor = Color(0);
      IOR = DEFAULT_DIFFUSE_IOR;
      emitter = false;
      emissionColor = Color(0);
    };
    
    Material( Color baseC, bool metal, bool specularity, float userRoughness, Color 
    			transmissionC=Color(0), float ior=1.5, Color emissC=Color(0) )
    {
      baseColor = baseC;
      metallic = metal;
      specular = specularity;
      roughness = userRoughness * userRoughness;
      if ((transmissionC.r > 0) || (transmissionC.g > 0) || (transmissionC.b > 0))
        translucent = true;
      else
        translucent = false;
      refractionColor = transmissionC;
      IOR = ior;
      if ((emissC.r > 0) || (emissC.g > 0) || (emissC.b > 0))
        emitter = true;
      else
        emitter = false;
      emissionColor = emissC;
    };
    
    virtual ~Material( ) {};


    // public methods
    Color GetReflectionColor( )
    {
      // FIXME
      return baseColor;
    };

    Color GetRefractionColor( )
    {
      // FIXME
      return refractionColor;
    };

    Color GetDiffuseColor( Vector raydir, Vector n_hit, Vector lightDirection )
    {
      // Simple Lambertian model. Note that we assume lightDirection points
      // *from* the light *to* the surface, so we need to reverse it for the
      // proper calculation
      return baseColor * fmax(float(0), Dot(n_hit, -lightDirection)) * ONE_OVER_PI;
    };

    Color GetEmissionColor( )
    {
      return emissionColor;
    };


    // public data members
    bool  metallic;
    bool  specular;
    bool  translucent;
    bool  emitter;
    float  IOR;

  private:
    // private data members
    Color  baseColor;
    float  roughness;
    Color  refractionColor;
    Color  emissionColor;
};


#endif  // _MATERIALS_H_
