#include "engine/core/Scene.hpp"
#include "engine/graphics/Billboard.hpp"
#include "engine/core/GameObject.hpp"
#include "engine/resources/ResourceManager.hpp" 
#include <string>

Scene::Scene() {
  init();
}

Scene::~Scene() {}

void Scene::addObject(GameObject* object)
{
  objects.push_back(object);
}

const std::vector<GameObject*>& Scene::getObjects() const
{
  return objects;
}

void Scene::update(float deltaTime) {
  for (const auto& obj_ptr : objects) { 
    if (obj_ptr && obj_ptr->isActive) {
      obj_ptr->updatePhysics(deltaTime);
    }
  }
}

const Skybox& Scene::getSkybox() const {
  if (!skybox) {
    throw std::runtime_error("[SCENE] Skybox não foi inicializado");
  }
  return *skybox;
}

void Scene::init()
{
  // SKYBOX
  skybox = new Skybox("assets/skybox");

  // OBJETOS (MODELOS)
  ResourceManager& manager = ResourceManager::getInstance();

  addObject(new GameObject( 
    "obj1",
    &manager.getShader("default"),
    &manager.getModel("pendurador"),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f),
    glm::vec3(2.0f, 2.0f, 2.0f)
   ));

  addObject(new GameObject( 
    "obj2",
    &manager.getShader("default"),
    &manager.getModel("bike"),
    glm::vec3(2.0f, 0.0f, 5.0f),
    glm::vec3(0.0f),
    glm::vec3(2.0f, 2.0f, 2.0f)
  ));

  addObject(new GameObject(
    "obj3",
    &manager.getShader("default"),
    &manager.getModel("casa1"),
    glm::vec3(10.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 180.0f, 0.0f),
    glm::vec3(0.01f, 0.01f, 0.01f)
  ));

  addObject(new GameObject( 
    "obj4",
    &manager.getShader("default"),
    &manager.getModel("pig"),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 180.0f, 0.0f),
    glm::vec3(1.0f)
  ));

  addObject(new Billboard(
    &manager.getShader("billboard"),
    &manager.getTexture("tree"),
    glm::vec3(1.0f, 1.0f, 2.0f),
    2.0f,
    glm::vec3(1.0f),
    "TreeBillboard1"
  ));

  addObject(new Billboard(
    &manager.getShader("billboard"),
    &manager.getTexture("tree"),
    glm::vec3(5.0f, 1.0f, 8.0f),
    2.0f,
    glm::vec3(1.0f),
    "TreeBillboard2"
  ));

  std::cout << "[SCENE] Cena inicializada com " << std::to_string(objects.size()) << " objetos." << std::endl;
}
