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
  terrain = new Terrain("assets/heightmaps/heightmap.png", "assets/heightmaps/mato.jpg", 1025, 1025);

  // OBJETOS (MODELOS)
  
  // Praça
  GameObject *praca = new GameObject(
    "plaza",
    &ResourceManager::getShader("default"),
    &ResourceManager::getModel("plaza"),
    glm::vec3(28.6f, -1.75f, 20.0f),
    glm::vec3(0.0f),
    glm::vec3(1.0f)
  );
  praca->colliderSize = glm::vec3(praca->colliderSize.x, 1.0f, praca->colliderSize.z);
  addObject(praca);

  GameObject* cantinho = new GameObject(
    "cantinho",
    &ResourceManager::getShader("default"),
    &ResourceManager::getModel("cantinho"),
    glm::vec3(25.0f, -0.9f, 14.0f),
    glm::vec3(0.0f),
    glm::vec3(3.0f)
  );

  cantinho->colliderSize = glm::vec3(cantinho->colliderSize.x, 0.5f, cantinho->colliderSize.z);
  addObject(cantinho);

  // TODO: otimizar essa desgraça   
  // Bancos
  float banco2_pos = 27.0f;
  for (int i = 0; i < 5; i++) {
    // Calçada (sidewalk) sob o banco
    float rot = (i % 2) == 0 ? 180.5f : -180.0f;

    GameObject *sidewalk = new GameObject(
      "sidewalk_banco_" + std::to_string(i),
      &ResourceManager::getShader("default"),
      &ResourceManager::getModel("sidewalk"),
      glm::vec3(18.0f, -0.26f, 7.8f * i),
      glm::vec3(-0.2f, rot, 0.0f),  // rotação
      glm::vec3(1.0f, 1.0f, 1.0f)   // escala padrão (ajuste conforme necessário)
    );
    sidewalk->isStatic = true;
    addObject(sidewalk);

    GameObject *banco = new GameObject(
      "banco" + std::to_string(i),
      &ResourceManager::getShader("default"),
      &ResourceManager::getModel("banco"),
      glm::vec3(20.0f, terrain->getHeight(20.0f, 5 * i) + 0.5f, 4.7f * i),
      glm::vec3(0.0f, -90.0f, 0.0f),
      glm::vec3(0.9f)
    );

    banco->colliderSize = glm::vec3(2.0f, 0.25f, 1.0f);
    addObject(banco);
    
    banco2_pos += (i >= 3) == 0 ? + 1.5f : -1.5f;
    GameObject *banco2 = new GameObject(
      "banco" + std::to_string(i),
      &ResourceManager::getShader("default"),
      &ResourceManager::getModel("banco"),
      glm::vec3(banco2_pos, terrain->getHeight(20.0f, 5 * i) + 0.5f, 4.7f * i),
      glm::vec3(0.0f, -270.0f, 0.0f),
      glm::vec3(0.9f)
    );

    banco2->colliderSize = glm::vec3(2.0f, 0.25f, 1.0f);
    addObject(banco2);

    addObject(new Billboard(
      &ResourceManager::getShader("billboard"),
      &ResourceManager::getTexture("mato" + std::to_string((i % 3) + 1)),
      glm::vec3(24.0, 1.0f, 0.0f + 4.9 * i),
      2.5f,
      glm::vec3(1.0f),
      "mato1_" + std::to_string(i)
    ));
  }
  
  // postes (billboard)
  for (int i = 0; i < 20; i++) {
    if ( i < 3) {
      addObject(new Billboard(
        &ResourceManager::getShader("billboard"),
        &ResourceManager::getTexture("poste"),
        glm::vec3(22.5f, 4.2f, 8 * i),
        10.0f,
        glm::vec3(1.0f),
        "poste" + std::to_string(i)
      ));
    }
    addObject(new Billboard(
      &ResourceManager::getShader("billboard"),
      &ResourceManager::getTexture("poste"),
      glm::vec3(12.5f, 4.0f, 10.0f + 20 * i),
      10.0f,
      glm::vec3(1.0f),
      "poste_pista" + std::to_string(i)
    ));
  }
  
  // coqueiros
  for (int i = 0; i < 20; i++) {
    GameObject * coco = new GameObject(
      "coconut" + std::to_string(i),
      &ResourceManager::getShader("default"),
      &ResourceManager::getModel("coconut"),
      glm::vec3(12.5f, terrain->getHeight(20.0f, 5 * i), 20 * i),
      glm::vec3(0.0f),
      glm::vec3(0.06f)
    );

    coco->colliderSize = glm::vec3(2.0f, 10.0f, 2.0f);
    addObject(coco);

    if (i < 3) {
      GameObject * coco2 = new GameObject(
        "coconut2" + std::to_string(i),
        &ResourceManager::getShader("default"),
        &ResourceManager::getModel("coconut"),
        glm::vec3(26.5f, terrain->getHeight(20.0f, 5 * i), 10 * i),
        glm::vec3(0.0f),
        glm::vec3(0.06f)
      );

      coco2->colliderSize = glm::vec3(2.0f, 10.0f, 2.0f);
      addObject(coco2); 
    }
  }

  // pistas
  for (int i = 0; i < 100; i++) {
    GameObject *obj = new GameObject(
      "pista" + std::to_string(i),
      &ResourceManager::getShader("default"),
      &ResourceManager::getModel("pista"),
      glm::vec3(15.0f, terrain->getHeight(15, 0.0f + 5.5 * i) + 0.01, 0.0f + 5.5 * i),
      glm::vec3(0.0f),
      glm::vec3(1.0f)
    );
  
    obj->isStatic = true;

    addObject(obj);

    obj = new GameObject(
      "pista2" + std::to_string(i),
      &ResourceManager::getShader("default"),
      &ResourceManager::getModel("pista"),
      glm::vec3(10.0f, terrain->getHeight(15, 0.0f + 5.5 * i) + 0.01, 0.0f + 5.5 * i),
      glm::vec3(0.0f),
      glm::vec3(1.0f)
    );

    obj->isStatic = true;

    addObject(obj);
  }
  
  for (int i = 0; i < 2; i++) {
    addObject(new GameObject(
      "mesa" + std::to_string(i),
      &ResourceManager::getShader("default"),
      &ResourceManager::getModel("mesa"),
      glm::vec3(33.0f, -0.5f, i * 2),
      glm::vec3(0.0f),
      glm::vec3(1.0f)
    ));

    addObject(new GameObject(
      "mesa2" + std::to_string(i),
      &ResourceManager::getShader("default"),
      &ResourceManager::getModel("mesa"),
      glm::vec3(35.0f, -0.5f, i * 2),
      glm::vec3(0.0f),
      glm::vec3(1.0f)
    ));
  }

  addObject(new GameObject(
    "igreja",
    &ResourceManager::getShader("default"),
    &ResourceManager::getModel("igreja"),
    glm::vec3(35.0f, -0.5f, 60.3f),
    glm::vec3(0.0f, 180.0f, 0.0f),
    glm::vec3(1.0f)
  ));

  addObject(new GameObject(
    "bus_stop",
    &ResourceManager::getShader("default"),
    &ResourceManager::getModel("parada_bus"),
    glm::vec3(19.0f, -0.25f, 30.0f),
    glm::vec3(0.0f, -90.0f, 0.0f),
    glm::vec3(1.0f)
  ));

  addObject(new GameObject(
    "donkey",
    &ResourceManager::getShader("default"),
    &ResourceManager::getModel("burro"),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(1.0f)
  ));

  std::srand(0);

  // arvores
  // float spacing = 4.0f;
  // for (int x = 0; x < 10; ++x) {
  //   for (int z = 0; z < 10; ++z) {
  //     float offsetX = (std::rand() % 200);
  //     float offsetZ = (std::rand() % 200);
  //
  //     float worldX = x * spacing + offsetX;
  //     float worldZ = z * spacing + offsetZ;
  //     float scale = 0.6f + static_cast<float>(std::rand()) / RAND_MAX * 0.1f;
  //
  //     addObject(new GameObject(
  //       "tree_" + std::to_string(x) + "_" + std::to_string(z),
  //       &ResourceManager::getShader("default"),
  //       &ResourceManager::getModel("tree"),
  //       glm::vec3(worldX, terrain->getHeight(worldX, worldZ), worldZ),
  //       glm::vec3(0.0f),
  //       glm::vec3(scale)
  //     ));
  //   }
  // }

  // matinho
  // for (int i = 0; i < 2000; i++) {
  //   float x = static_cast<float>(std::rand() % 300); 
  //   float z = static_cast<float>(std::rand() % 300);
  //   float baseHeight = terrain->getHeight(x, z) - 0.15f;
  //
  //   float scale = 0.3f + static_cast<float>(std::rand() % 40) / 100.0f;
  //   glm::vec3 position(x, baseHeight, z);
  //   glm::vec3 scaleVec(scale);
  //
  //   std::string namePrefix = "grass_" + std::to_string(i);
  //
  //   for (int j = 0; j < 5; j++) {
  //     float offsetX = ((std::rand() % 10) - 5) * 0.08f;
  //     float offsetZ = ((std::rand() % 10) - 5) * 0.08f;
  //
  //     float rotY = static_cast<float>(std::rand() % 360);
  //
  //     GameObject *obj = new GameObject(
  //       namePrefix + "_patch_" + std::to_string(j),
  //       &ResourceManager::getShader("default"),
  //       &ResourceManager::getModel("grass"),
  //       glm::vec3(x + offsetX, baseHeight, z + offsetZ),
  //       glm::vec3(0.0f, rotY, 0.0f),
  //       scaleVec
  //     );
  //
  //     obj->isStatic = true;
  //
  //     addObject(obj); 
  //   }
  // }  
  //
  std::cout << "[SCENE] Cena inicializada com " << std::to_string(objects.size()) << " objetos." << std::endl;
}
