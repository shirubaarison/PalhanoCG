#include "core/Scene.hpp"
#include "resources/ResourceManager.hpp"

Scene::Scene() {}

void Scene::addObject(const GameObject& object) 
{
  objects.push_back(object);
}

const std::vector<GameObject>& Scene::getObjects() const 
{
  return objects;
}

void Scene::update(float deltaTime) { }

const Skybox& Scene::getSkybox() const { return *skybox; }

void Scene::init() 
{
  // SKYBOX
  skybox = new Skybox("assets/skybox");
  
  // OBJETOS (MODELOS)
  ResourceManager& manager = ResourceManager::getInstance();

  GameObject obj1;

  obj1.shader = &manager.getShader("default");
  obj1.model = &manager.getModel("pendurador");
  obj1.transform.position = {0.0f, 0.0f, 0.0f};
  obj1.transform.scale = {2.0f, 2.0f, 2.0f};
  obj1.colliderSize = obj1.model->aabbSize;

  addObject(obj1);

  GameObject obj2;

  obj2.shader = &manager.getShader("default");
  obj2.model = &manager.getModel("bike");
  obj2.transform.position = {2.0f, 0.0f, 5.0f};
  obj2.transform.scale = {2.0f, 2.0f, 2.0f};
  obj2.colliderSize = obj2.model->aabbSize;

  addObject(obj2);

  GameObject obj3;

  obj3.shader = &manager.getShader("default");
  obj3.model = &manager.getModel("casa1");
  obj3.transform.rotation = {0.0f, 180.0f, 0.0f};
  obj3.transform.position = {10.0f, 0.0f, 0.0f};
  obj3.transform.scale = {0.01f, 0.01f, 0.01f};
  obj3.colliderSize = obj3.model->aabbSize;

  addObject(obj3);

  GameObject obj4;

  obj4.shader = &manager.getShader("default");
  obj4.model = &manager.getModel("pig");
  obj4.transform.rotation = {0.0f, 180.0f, 0.0f};
  obj4.transform.position = {0.0f, 0.0f, 0.0f};
  obj4.colliderSize = obj4.model->aabbSize;

  addObject(obj4);
}
