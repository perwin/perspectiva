// Header file for Sampler subclass which does uniform subsampling within
// each pixel.

#ifndef _UNIFORM_SAMPLER_H_
#define _UNIFORM_SAMPLER_H_

#include "sampler.h"


class UniformSampler : public Sampler
{ 
public:

  // default constructor and destructor, redefined
  UniformSampler( const int sampleRate=1 );
  ~UniformSampler( );

  // No need to redefine do-nothing Update method
  
  /// Computes offset in x and y xOffset, yOffset for sample number n
  void GetOffsetCoords( const int n, float *xOffset, float *yOffset ) const;
}; 


#endif  // _UNIFORM_SAMPLER_H_
