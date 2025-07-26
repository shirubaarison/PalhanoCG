#include "engine/core/Game.hpp"
#include "engine/core/Scene.hpp"
#include "engine/graphics/Shader.hpp"
#include "engine/input/InputHandler.hpp"
#include "engine/utils/Globals.hpp"
#include "engine/resources/ResourceManager.hpp"

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

      glm::vec3 objMin = obj->getAABBMin();
      glm::vec3 objMax = obj->getAABBMax();

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

        // Aplique a nova posição ao jogador
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
    ui->drawSprite(ResourceManager::getTexture("mao"), glm::vec2(Globals::WIDTH - 256.0f, (Globals::HEIGHT - 200.0f)), glm::vec2(256.0f, 256.0f), 150.0f);
  glEnable(GL_DEPTH_TEST); 
}

void Game::loadAssets()
{
	// Shaders
  ResourceManager::loadShader("default", "shaders/default.vs.glsl", "shaders/default.fs.glsl");
	ResourceManager::loadShader("terrain", "shaders/terrain.vs.glsl", "shaders/terrain.fs.glsl");
	ResourceManager::loadShader("skybox", "shaders/skybox.vs.glsl", "shaders/skybox.fs.glsl");
  ResourceManager::loadShader("ui", "shaders/sprite_ui.vs.glsl", "shaders/sprite_ui.fs.glsl");
  ResourceManager::loadShader("billboard", "shaders/billboard.vs.glsl", "shaders/billboard.fs.glsl");

	// Modelos	
  ResourceManager::loadModel("pendurador", "assets/models/suporte/suporte.obj");
  ResourceManager::loadModel("tree", "assets/models/tree/beech_tree.obj");
  ResourceManager::loadModel("grass", "assets/models/grass/grass.obj");
  ResourceManager::loadModel("banco", "assets/models/banco/classic_park_bench_low_poly.obj");
  ResourceManager::loadModel("pista", "assets/models/pista/pista.obj");
  ResourceManager::loadModel("coconut", "assets/models/coconut/coconut_tree_low_poly.obj");
  ResourceManager::loadModel("sidewalk", "assets/models/sidewalk_8m_long_-_hq/sidewalk_8m_long_-_hq.obj");
  ResourceManager::loadModel("plaza", "assets/models/plaza/praca.obj");
  ResourceManager::loadModel("cantinho", "assets/models/cantinho/matinho.obj");
  ResourceManager::loadModel("mesa", "assets/models/mesa/stone_table_01.obj");
  ResourceManager::loadModel("parada_bus", "assets/models/bus_stop/brazilian_bus_stop.obj");
  ResourceManager::loadModel("burro", "assets/models/burro/donkey.obj");

  ResourceManager::loadModel("casa1", "assets/models/psx_house/psx_house.obj");
  ResourceManager::loadModel("igreja", "assets/models/psx_abandoned_church/psx_abandoned_church.obj");
  ResourceManager::loadModel("3em1", "assets/models/3em1/3 em 1.obj"); 
  ResourceManager::loadModel("bike", "assets/models/bike/bike.obj");
  ResourceManager::loadModel("sedan", "assets/models/sedan/psx_sedan_car_-_jonniemadeit.obj");
  ResourceManager::loadModel("lixo", "assets/models/trash_can/trash_can.obj");
  ResourceManager::loadModel("churrasqueira", "assets/models/churrasqueira/commercial_prop_for_brazilian_street_food_-_68.obj");
  ResourceManager::loadModel("caminhador", "assets/models/caminhador/caminhador.obj");
  ResourceManager::loadModel("pendurador_cima", "assets/models/pendurador_cima/parte de cima.obj");

  // Sprites para a UI
  ResourceManager::loadTexture("crosshair", "assets/sprites/crosshair.png");
  ResourceManager::loadTexture("mao", "assets/sprites/steve-hand.png");

  // Billboards
  ResourceManager::loadTexture("poste", "assets/sprites/poste.png");
  ResourceManager::loadTexture("mato1", "assets/sprites/mato1.png");
  ResourceManager::loadTexture("mato2", "assets/sprites/mato2.png");
  ResourceManager::loadTexture("mato3", "assets/sprites/mato3.png");
}
