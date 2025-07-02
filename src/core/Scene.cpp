#include "core/Scene.hpp"
#include "resources/ResourceManager.hpp"

void Scene::addObject(const GameObject& object) 
{
  objects.push_back(object);
  objectPointers.push_back(&objects.back());
}

const std::vector<GameObject>& Scene::getObjects() const {
  return objects;
}

void Scene::update(float deltaTime) 
{
  for (auto& obj : objects) {
    if (obj.isActive) {
      // idk
    }
  }
}

void Scene::init() 
{
  ResourceManager& manager = ResourceManager::getInstance();
  
  GameObject obj1;

  obj1.shader = &manager.getShader("default");
  obj1.model = &manager.getModel("saitama");
  obj1.transform.position = {0.0f, 0.0f, 0.0f};
  obj1.transform.rotation = {90.0f, 0.0f, 0.0f};

  addObject(obj1);

  GameObject obj2;

  obj2.shader = &manager.getShader("default");
  obj2.model = &manager.getModel("pig");
  obj2.transform.position = {2.0f, 0.0f, 5.0f};
  obj2.transform.scale = {10.0f, 10.0f, 10.0f};

  addObject(obj2);
}
