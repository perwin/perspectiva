#include "sampler.h"

Sampler::Sampler( const int sampleRate )
{
  nSamples1D = sampleRate;
  nSamples2D = nSamples1D*nSamples1D;
  offsetsAllocated = false;
}


/// To be redefined in other classes or subclasses (e.g., for random sampling,
/// we want to compute new values for xOffsets and yOffsets whenever this is
/// called
void Sampler::Update( )
{
  ;
}


Sampler::~Sampler( )
{
  ;
}


/// Simplest possible case: no oversampling, so always set offsets = 0.0
void Sampler::GetOffsetCoords( const int n, float *xOffset, float *yOffset ) const
{
  
  *xOffset = 0.0;
  *yOffset = 0.0;
}

