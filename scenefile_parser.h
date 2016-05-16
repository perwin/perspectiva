#ifndef _SCENEFILE_PARSER_H_
#define _SCENEFILE_PARSER_H_

#include <string>

bool VetSceneFile( const std::string sceneFilename );

Scene* LoadSceneFromFile( std::string sceneFilename, int debugLevel=0 );

#endif  // _SCENEFILE_PARSER_H_
