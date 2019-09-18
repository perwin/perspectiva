#ifndef _ENVIRONMENT_MAP_H_
#define _ENVIRONMENT_MAP_H_

#include <cstdlib> 
#include <math.h>

#include "color.h"


class Environment
{
  public:
    
    // Constructors with initializations:
    Environment( ) { baseColor = Color(0); hasMap = false; }
    Environment( const Color c ) { baseColor = c; hasMap = false; }

    ~Environment( ) { ; }

	void SetEnvironmentColor( Color c) { baseColor = c; }
	
	Color GetEnvironmentColor( Ray theRay )
	{
	  if (hasMap)
	    return Color(0);  // dummy action; to be replaced with map sampling
	  else
	    return baseColor;
	}

    // Data members:
	Color  baseColor;
	bool  hasMap;
};




#endif  // _ENVIRONMENT_MAP_H_
