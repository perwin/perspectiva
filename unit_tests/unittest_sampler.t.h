// Unit tests for code in sampler.cpp

#include <cxxtest/TestSuite.h>
#include "sampler.h"



class NewTestSuite : public CxxTest::TestSuite 
{
public:

  // NOTE: the following will not work if we make the Sampler internal variable private
  void testCreation( void )
  {
    int nsamples, nsamples2D;
    
    Sampler *sampler1 = new Sampler();
    Sampler *sampler3 = new Sampler(3);
    
    nsamples = sampler1->nSamples1D;
    TS_ASSERT(nsamples == 1);
    nsamples2D = sampler1->nSamples2D;
    TS_ASSERT(nsamples2D == 1);

    nsamples = sampler3->nSamples1D;
    TS_ASSERT(nsamples == 3);
    nsamples2D = sampler3->nSamples2D;
    TS_ASSERT(nsamples2D == 9);
    
    delete sampler1;
    delete sampler3;
  }

  void testSampling_xVals( void )
  {    
    Sampler sampler3x3 = Sampler(3);
    float  xx,yy;
    
    float xOffCorrect[9] = {-0.33333333, 0.0, 0.3333333, -0.33333333, 0.0, 0.3333333, 
    						-0.33333333, 0.0, 0.3333333};
    float result[9] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    for (int n = 0; n < 9; ++n) {
      sampler3x3.GetOffsetCoords(n, &xx, &yy);
      result[n] = xx;
    }
    for (int i = 0; i < 9; ++i)
      TS_ASSERT_DELTA(result[i], xOffCorrect[i], 1.0e-6);    
  }

  void testSampling_yVals( void )
  {    
    Sampler *sampler3x3 = new Sampler(3);
    float  xx,yy;
    
    float yOffCorrect[9] = {-0.33333333, -0.33333333, -0.33333333, 0.0, 0.0, 0.0,
    						0.3333333, 0.33333333, 0.3333333};
    float result[9] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    for (int n = 0; n < 9; ++n) {
      sampler3x3->GetOffsetCoords(n, &xx, &yy);
      result[n] = yy;
    }
    for (int i = 0; i < 9; ++i)
      TS_ASSERT_DELTA(result[i], yOffCorrect[i], 1.0e-6);
    
    delete sampler3x3;
  }


};
