#ifndef _SCENEFILE_PARSER_H_
#define _SCENEFILE_PARSER_H_

#include <string>
#include "yaml-cpp/yaml.h"
#include "scene.h"

bool VetSceneFile( const std::string &sceneFilename );

float GetFileVersion( const std::string &sceneFilename );

void AddSphereToScene( YAML::Node sphereNode, shared_ptr<Scene> theScene, const int debugLevel=1 );

void AddBoxToScene( YAML::Node objNode, shared_ptr<Scene> theScene, const int debugLevel=0 );

void AddPlaneToScene( YAML::Node objNode, shared_ptr<Scene> theScene, const int debugLevel=0 );

void AddLightToScene( YAML::Node objNode, shared_ptr<Scene> theScene, const int debugLevel=0 );

void AddMaterialToScene( YAML::Node objNode, shared_ptr<Scene> theScene, const int debugLevel=1 );

void AddBackgroundToScene( YAML::Node objNode, shared_ptr<Scene> theScene, const int debugLevel=0 );

void AddAtmosphereToScene( YAML::Node atmNode, shared_ptr<Scene> theScene, const int debugLevel=0 );

void AddCameraToScene( YAML::Node objNode, shared_ptr<Scene> theScene, const int debugLevel=0 );

shared_ptr<Scene> LoadSceneFromFile( const std::string &sceneFilename, const int debugLevel=0 );

#endif  // _SCENEFILE_PARSER_H_
