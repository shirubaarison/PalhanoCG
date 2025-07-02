#include "core/Scene.hpp"

void Scene::addObject(const std::shared_ptr<GameObject>& object) 
{
  objects.push_back(object);
}


void Scene::removeObject(const std::shared_ptr<GameObject>& object) 
{
  objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
}

const std::vector<std::shared_ptr<GameObject>>& Scene::getObjects() const    
{
    return objects;
}

void Scene::update(float deltaTime) 
{
  for (auto& obj : objects) {
    if (obj->isActive) {
      // idk
    }
  }
}
