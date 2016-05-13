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

/// struct for holding various general options (set by command-line flags & options)
/// Note that we initialize things inside the definition, which requires C++11
typedef struct {
  bool  noImageName = true;
  bool  imageSizeSet = false;
  bool  saveAlpha = false;
  unsigned imageWidth = 0;
  unsigned imageHeight = 0;
  std::string  outputImageName = DEFAULT_OUTPUT_IMAGE_FILENAME;
} commandOptions;


#endif  // _COMMAND_OPTION_STRUCT_H_
