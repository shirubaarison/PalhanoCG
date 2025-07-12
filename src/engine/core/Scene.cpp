#include "engine/core/Scene.hpp"
#include "engine/core/Game.hpp"
#include "engine/game/world/Terrain.hpp"
#include "engine/graphics/Billboard.hpp"
#include "engine/core/GameObject.hpp"
#include "engine/resources/ResourceManager.hpp" 
#include <stdexcept>
#include <string>
#include <cstdlib> 
#include <ctime>

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
      obj_ptr->updatePhysics(terrain, deltaTime);
    }
  }
}

const Skybox& Scene::getSkybox() const 
{
  if (!skybox) {
    throw std::runtime_error("[SCENE] Skybox não foi inicializado");
  }
  return *skybox;
}

const Terrain& Scene::getTerrain() const 
{
  if (!terrain) {
    throw std::runtime_error("[SCENE] Terreno não foi inicializado");
  }

  return *terrain;
}

void Scene::init()
{
  // SKYBOX
  skybox = new Skybox("assets/skybox");

  // TERRENO 
  terrain = new Terrain("assets/heightmaps/heightmap.png", "assets/heightmaps/areia.jpg", 1025, 1025);

  // OBJETOS (MODELOS)
  ResourceManager& manager = ResourceManager::getInstance();

  addObject(new GameObject(
    "casa1",
    &manager.getShader("default"),
    &manager.getModel("casa1"),
    glm::vec3(10.0f, -0.55f, 0.0f),
    glm::vec3(0.0f, 180.0f, 0.0f),
    glm::vec3(0.01f, 0.01f, 0.01f)
  ));

  addObject(new GameObject(
    "pendurador",
    &manager.getShader("default"),
    &manager.getModel("pendurador"),
    glm::vec3(10.0f, 0.0f, 10.0f),
    glm::vec3(0.0f),
    glm::vec3(4.0f)
  ));

  std::srand(0);

  int countX = 5;
  int countZ = 5;
  float spacing = 4.0f;

  for (int x = 0; x < countX; ++x) {
    for (int z = 0; z < countZ; ++z) {
      float offsetX = (std::rand() % 200);
      float offsetZ = (std::rand() % 200);

      float worldX = x * spacing + offsetX;
      float worldZ = z * spacing + offsetZ;
      float scale = 0.6f + static_cast<float>(std::rand()) / RAND_MAX * 0.1f;

      addObject(new GameObject(
        "tree_" + std::to_string(x) + "_" + std::to_string(z),
        &manager.getShader("default"),
        &manager.getModel("tree"),
        glm::vec3(worldX, terrain->getHeight(worldX, worldZ), worldZ),
        glm::vec3(0.0f),
        glm::vec3(scale)
      ));
    }
  }

  // matinho
  for (int i = 0; i < 2000; i++) {
    float x = static_cast<float>(std::rand() % 300); 
    float z = static_cast<float>(std::rand() % 300);
    float baseHeight = terrain->getHeight(x, z) - 0.15f;

    float scale = 0.3f + static_cast<float>(std::rand() % 40) / 100.0f;
    glm::vec3 position(x, baseHeight, z);
    glm::vec3 scaleVec(scale);

    std::string namePrefix = "grass_" + std::to_string(i);

    for (int j = 0; j < 3; j++) {
      float offsetX = ((std::rand() % 10) - 5) * 0.08f;
      float offsetZ = ((std::rand() % 10) - 5) * 0.08f;

      float rotY = static_cast<float>(std::rand() % 360);
      
      GameObject *obj = new GameObject(
        namePrefix + "_patch_" + std::to_string(j),
        &manager.getShader("default"),
        &manager.getModel("grass"),
        glm::vec3(x + offsetX, baseHeight, z + offsetZ),
        glm::vec3(0.0f, rotY, 0.0f),
        scaleVec
      );
      
      obj->isStatic = true;

      addObject(obj); 
    }
  }  

  std::cout << "[SCENE] Cena inicializada com " << std::to_string(objects.size()) << " objetos." << std::endl;
}
