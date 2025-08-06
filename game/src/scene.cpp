#include "scene.h"
#include "resource_manager.h"



Scene::Scene(std::string &sSceneFile)
{
    RM::LoadSceneData(*this, sSceneFile);
}



Scene::~Scene() {}
