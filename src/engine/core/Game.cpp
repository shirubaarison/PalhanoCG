#include "engine/core/Game.hpp"
#include "engine/core/Scene.hpp"
#include "engine/input/InputHandler.hpp"
#include "engine/utils/Globals.hpp"

#include <iostream>
#include <glm/glm.hpp>

Game::Game() 
  : state(GAME_ACTIVE),
    resourceManager(ResourceManager::getInstance())
    {}

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
  player->update(deltaTime, scene->getTerrain());
  scene->update(deltaTime);
  
  if (player) {
    glm::vec3 playerMin = player->getAABBMin();
    glm::vec3 playerMax = player->getAABBMax();

    const std::vector<GameObject*> sceneObjects = scene->getObjects();
    for (GameObject *obj : sceneObjects) {
      if (obj->isStatic) {
        continue;
      }

      glm::vec3 objMin = obj->transform.position - (obj->colliderSize * obj->transform.scale * 0.5f);
      glm::vec3 objMax = obj->transform.position + (obj->colliderSize * obj->transform.scale * 0.5f);

      bool overlapX = (playerMax.x > objMin.x) && (playerMin.x < objMax.x);
      bool overlapY = (playerMax.y > objMin.y) && (playerMin.y < objMax.y);
      bool overlapZ = (playerMax.z > objMin.z) && (playerMin.z < objMax.z);

      if (overlapX && overlapY && overlapZ) {
        float overlapX_depth = std::min(playerMax.x, objMax.x) - std::max(playerMin.x, objMin.x);
        float overlapY_depth = std::min(playerMax.y, objMax.y) - std::max(playerMin.y, objMin.y);
        float overlapZ_depth = std::min(playerMax.z, objMax.z) - std::max(playerMin.z, objMin.z);

        if (overlapX_depth < overlapY_depth && overlapX_depth < overlapZ_depth) {
          if (playerMin.x < objMin.x) { // esquerdo
            player->pCamera.position.x -= overlapX_depth;
          } else { // direita
            player->pCamera.position.x += overlapX_depth;
          }
          player->velocity.x = 0;
        } else if (overlapY_depth < overlapX_depth && overlapY_depth < overlapZ_depth) {
          if (playerMin.y < objMin.y) { // abaixo
            player->pCamera.position.y -= overlapY_depth;
          } else { // acima
            player->pCamera.position.y += overlapY_depth;
            player->isOnGround = true;
          }
          player->velocity.y = 0;
        } else {
          if (playerMin.z < objMin.z) { // frente
            player->pCamera.position.z -= overlapZ_depth;
          } else { // atras
            player->pCamera.position.z += overlapZ_depth;
          }
          player->velocity.z = 0;
        }
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
    ui->drawSprite(ResourceManager::getInstance().getTexture("crosshair"), glm::vec2((Globals::WIDTH - 32.0f) / 2.0f, (Globals::HEIGHT - 32.0f) / 2.0f), glm::vec2(32.0f, 32.0f));
  glEnable(GL_DEPTH_TEST); 
}

void Game::loadAssets()
{
	// Shaders
	resourceManager.loadShader("default", "assets/shaders/default.vs.glsl", "assets/shaders/default.fs.glsl");
	resourceManager.loadShader("terrain", "assets/shaders/terrain.vs.glsl", "assets/shaders/terrain.fs.glsl");
	resourceManager.loadShader("skybox", "assets/shaders/skybox.vs.glsl", "assets/shaders/skybox.fs.glsl");
  resourceManager.loadShader("ui", "assets/shaders/sprite_ui.vs.glsl", "assets/shaders/sprite_ui.fs.glsl");
  resourceManager.loadShader("billboard", "assets/shaders/billboard.vs.glsl", "assets/shaders/billboard.fs.glsl");

	// Modelos	
  resourceManager.loadModel("casa1", "assets/models/casa1/model.obj");
  resourceManager.loadModel("pendurador", "assets/models/pendurador/da p se pendurar.obj");
  resourceManager.loadModel("tree", "assets/models/tree/stylized_hand-painted_tree.obj");

  // Sprites para a UI
  resourceManager.loadTexture("crosshair", "assets/sprites/crosshair.png");

  // Billboarding
  resourceManager.loadTexture("bush", "assets/sprites/bush.png");
}
