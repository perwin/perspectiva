/*! \file
    \brief Struct containing options info for raytracer2, along with function for
           setting up default values within the struct.
 *
 */

#ifndef _COMMAND_OPTION_STRUCT_H_
#define _COMMAND_OPTION_STRUCT_H_

#include <string>
#include <vector>

const string DEFAULT_OUTPUT_IMAGE_FILENAME = "untitled";

// Note: it would be nice to be able to initialize the various members of the
// commandOptions struct in the actual definition, as C++11 allows;
// then we could get rid of the SetDefaultMakeimageOptions() function.

/// struct for holding various makeimage options (set by command-line flags & options)
typedef struct {
  bool  noImageName;
  std::string  outputImageName;
} commandOptions;



void SetDefaultOptions( commandOptions *theOptions )
{
  theOptions->noImageName = true;
  theOptions->outputImageName = DEFAULT_OUTPUT_IMAGE_FILENAME;
}



#endif  // _COMMAND_OPTION_STRUCT_H_
