#ifndef _SCENEFILE_PARSER_H_
#define _SCENEFILE_PARSER_H_

#include <string>
#include "yaml-cpp/yaml.h"
#include "scene.h"

bool VetSceneFile( const std::string sceneFilename );

float GetFileVersion( const std::string sceneFilename );

void AddSphereToScene( YAML::Node objNode, Scene *theScene, int debugLevel=0 );

void AddPlaneToScene( YAML::Node objNode, Scene *theScene, int debugLevel=0 );

void AddLightToScene( YAML::Node objNode, Scene *theScene, int debugLevel=0 );

void AddBackgroundToScene(YAML::Node objNode, Scene *theScene, int debugLevel=0);

Scene* LoadSceneFromFile( std::string sceneFilename, int debugLevel=0 );

#endif  // _SCENEFILE_PARSER_H_
