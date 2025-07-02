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

// const Skybox& Scene::getSkybox() const { return skybox; }

void Scene::init() 
{
  // TERRENO
  terrain = new Terrain("assets/heightmaps/heightmap.png", "assets/heightmaps/mato.jpg", 1025, 1025);

  // SKYBOX
  // skybox = Skybox("assets/skybox");
  
  // OBJETOS (MODELOS)
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
