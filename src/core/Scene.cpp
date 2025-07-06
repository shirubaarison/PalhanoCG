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

void Scene::update(float deltaTime) 
{
  for (auto& obj : objects) {
    if (obj.isActive) {
      // idk
    }
  }
}

const Terrain& Scene::getTerrain() const { return *terrain; }

const Skybox& Scene::getSkybox() const { return *skybox; }

void Scene::init() 
{
  // TERRENO
  terrain = new Terrain("assets/heightmaps/heightmap.png", "assets/heightmaps/areia.jpg", 1025, 1025);

  // SKYBOX
  skybox = new Skybox("assets/skybox");
  
  // OBJETOS (MODELOS)
  ResourceManager& manager = ResourceManager::getInstance();

  GameObject obj1;

  obj1.shader = &manager.getShader("default");
  obj1.model = &manager.getModel("pendurador");
  obj1.transform.position = {0.0f, 0.0f, 0.0f};
  obj1.transform.scale = {2.0f, 2.0f, 2.0f};

  addObject(obj1);

  GameObject obj2;

  obj2.shader = &manager.getShader("default");
  obj2.model = &manager.getModel("bike");
  obj2.transform.position = {2.0f, 2.0f, 5.0f};
  obj2.transform.scale = {2.0f, 2.0f, 2.0f};

  addObject(obj2);

  GameObject obj3;

  obj3.shader = &manager.getShader("default");
  obj3.model = &manager.getModel("casa1");
  obj3.transform.rotation = {0.0f, 180.0f, 0.0f};
  obj3.transform.position = {10.0f, 0.0f, 0.0f};
  obj3.transform.scale = {0.01f, 0.01f, 0.01f};

  addObject(obj3);
  
  GameObject obj4;

  obj4.shader = &manager.getShader("default");
  obj4.model = &manager.getModel("pig");
  obj4.transform.rotation = {0.0f, 180.0f, 0.0f};
  obj4.transform.position = {0.0f, 3.0f, 0.0f};
  obj4.transform.scale = {10.0f, 10.0f, 10.0f};

  addObject(obj4);
}
