// Header file for Sampler subclass which does jittered uniform subsampling within
// each pixel.

#ifndef _UNIFORM_JITTER_SAMPLER_H_
#define _UNIFORM_JITTER_SAMPLER_H_

#include "sampler.h"


class UniformJitterSampler : public Sampler
{ 
public:

  // default constructor and destructor, redefined
  UniformJitterSampler( const int sampleRate=1 );
  ~UniformJitterSampler( );

  /// Computes offset in x and y xOffset, yOffset for sample number n
  void GetOffsetCoords( const int n, float *xOffset, float *yOffset ) const;
  
private:
  float  pixFrac;
}; 


#endif  // _UNIFORM_JITTER_SAMPLER_H_
