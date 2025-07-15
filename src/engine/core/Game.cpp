#include "engine/core/Game.hpp"
#include "engine/core/Scene.hpp"
#include "engine/graphics/Shader.hpp"
#include "engine/input/InputHandler.hpp"
#include "engine/utils/Globals.hpp"

#include <iostream>
#include <glm/glm.hpp>

Game::Game() : state(GAME_ACTIVE) {}

bool Game::initialize()
{
	// Inicializar janelas
  window = new Window();
	if (!window->initialize()) {
		std::cerr << "Erro ao inicializar Window" << std::endl;
		return false;
	}

	// Inicializar Inputs
	input = new InputHandler();
  if (!input->initialize(window->getWindow())) {
    std::cerr << "Erro ao inicializar Input" << std::endl;
  }

	// Inicializar OpenGL
  renderer = new Renderer();
  if (!renderer->init()) {
    std::cerr << "Erro ao inicializar OpenGL" << std::endl;
    return false;
  }
  
  // Carregue assets (shaders, texturas, modelos...)
  loadAssets();
	
  // Inicializar o player
	player = new Player(input);

  // Inicializar a cena 
  scene = new Scene();
	
  // Inicializar UI 
  ui = new UI();

	return true;
}

void Game::shutdown()
{
	delete player;
	player = nullptr;

  // TODO: adicionar mais coisas
}

void Game::run()
{
	float deltaTime = 0.0f;
  float mLastFrame = 0.0f;
	
	while (!window->windowShouldClose()) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - mLastFrame;
		mLastFrame = currentFrame;

		update(deltaTime);
		render();

		glfwPollEvents();
		window->swapBuffers();
	}
}

void Game::update(float deltaTime) 
{
  player->pCamera.updateFrustum();
  player->update(deltaTime, scene->getTerrain());
  scene->update(deltaTime);
  
  if (player) {
    glm::vec3 playerMin = player->getAABBMin();
    glm::vec3 playerMax = player->getAABBMax();

    const std::vector<GameObject*> sceneObjects = scene->getObjects();
    for (GameObject* obj : sceneObjects) {
      if (obj->isStatic)
        continue;

      glm::vec3 objMin = obj->transform.position - (obj->colliderSize * obj->transform.scale * 0.5f);
      glm::vec3 objMax = obj->transform.position + (obj->colliderSize * obj->transform.scale * 0.5f);

      bool overlapX = (playerMax.x > objMin.x) && (playerMin.x < objMax.x);
      bool overlapY = (playerMax.y > objMin.y) && (playerMin.y < objMax.y);
      bool overlapZ = (playerMax.z > objMin.z) && (playerMin.z < objMax.z);

      if (overlapX && overlapY && overlapZ) {
        float overlapX_depth = std::min(playerMax.x, objMax.x) - std::max(playerMin.x, objMin.x);
        float overlapY_depth = std::min(playerMax.y, objMax.y) - std::max(playerMin.y, objMin.y);
        float overlapZ_depth = std::min(playerMax.z, objMax.z) - std::max(playerMin.z, objMin.z);

        glm::vec3 playerPos = player->pCamera.getPosition();

        if (overlapX_depth < overlapY_depth && overlapX_depth < overlapZ_depth) {
          if (playerMin.x < objMin.x)
            playerPos.x -= overlapX_depth;
          else
            playerPos.x += overlapX_depth;
          player->velocity.x = 0.0f;
        } 
        else if (overlapY_depth < overlapX_depth && overlapY_depth < overlapZ_depth) {
          if (playerMin.y < objMin.y)
            playerPos.y -= overlapY_depth;
          else {
            playerPos.y += overlapY_depth;
            player->isOnGround = true;
          }
          player->velocity.y = 0.0f;
        } 
        else {
          if (playerMin.z < objMin.z)
            playerPos.z -= overlapZ_depth;
          else
            playerPos.z += overlapZ_depth;
          player->velocity.z = 0.0f;
        }

        player->pCamera.setPosition(playerPos);
      }
    }
  }
}

void Game::render()
{
  // Objetos 3D, terreno, billboard e skybox
  renderer->render(*scene, player->getCamera());

  // UI
  glDisable(GL_DEPTH_TEST);
    ui->drawSprite(ResourceManager::getTexture("crosshair"), glm::vec2((Globals::WIDTH - 32.0f) / 2.0f, (Globals::HEIGHT - 32.0f) / 2.0f), glm::vec2(32.0f, 32.0f));
  glEnable(GL_DEPTH_TEST); 
}

void Game::loadAssets()
{
	// Shaders
  ResourceManager::loadShader("default", "assets/shaders/default.vs.glsl", "assets/shaders/default.fs.glsl");
	ResourceManager::loadShader("terrain", "assets/shaders/terrain.vs.glsl", "assets/shaders/terrain.fs.glsl");
	ResourceManager::loadShader("skybox", "assets/shaders/skybox.vs.glsl", "assets/shaders/skybox.fs.glsl");
  ResourceManager::loadShader("ui", "assets/shaders/sprite_ui.vs.glsl", "assets/shaders/sprite_ui.fs.glsl");
  ResourceManager::loadShader("billboard", "assets/shaders/billboard.vs.glsl", "assets/shaders/billboard.fs.glsl");

	// Modelos	
  ResourceManager::loadModel("pendurador", "assets/models/pendurador/da p se pendurar.obj");
  ResourceManager::loadModel("tree", "assets/models/tree/beech_tree.obj");
  ResourceManager::loadModel("grass", "assets/models/grass/grass.obj");
  ResourceManager::loadModel("banco", "assets/models/banco/classic_park_bench_low_poly.obj");
  ResourceManager::loadModel("poste1", "assets/models/poste1/7_12_2025(1).obj");
  ResourceManager::loadModel("pista", "assets/models/pista/pista.obj");
  ResourceManager::loadModel("coconut", "assets/models/coconut/coconut_tree_low_poly.obj");
  ResourceManager::loadModel("sidewalk", "assets/models/sidewalk_8m_long_-_hq/sidewalk_8m_long_-_hq.obj");
  ResourceManager::loadModel("plaza", "assets/models/plaza/praca.obj");
  ResourceManager::loadModel("cantinho", "assets/models/cantinho/matinho.obj");
  ResourceManager::loadModel("mesa", "assets/models/mesa/stone_table_01.obj");
  ResourceManager::loadModel("igreja", "assets/models/igreja/church.obj");
  ResourceManager::loadModel("parada_bus", "assets/models/bus_stop/brazilian_bus_stop.obj");
  ResourceManager::loadModel("burro", "assets/models/burro/donkey.obj");

  // Sprites para a UI
  ResourceManager::loadTexture("crosshair", "assets/sprites/crosshair.png");

  // Billboards
  ResourceManager::loadTexture("poste", "assets/sprites/poste.png");
  ResourceManager::loadTexture("mato1", "assets/sprites/mato1.png");
  ResourceManager::loadTexture("mato2", "assets/sprites/mato2.png");
  ResourceManager::loadTexture("mato3", "assets/sprites/mato3.png");
}
