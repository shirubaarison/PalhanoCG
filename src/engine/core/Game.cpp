#include "engine/core/Game.hpp"
#include "engine/graphics/SpriteRenderer.hpp"
#include "engine/input/InputHandler.hpp"

#include <iostream>
#include <glm/glm.hpp>

unsigned int WIDTH = 1280;
unsigned int HEIGHT = 720;
const char* TITLE = "Trabalho CG";

Game::Game() 
  : state(GAME_ACTIVE),
    resourceManager(ResourceManager::getInstance()),
    window(),
    gIsRunning(false) {}

bool Game::initialize()
{
	// Inicializar janelas
	if (!window.initialize(WIDTH, HEIGHT, TITLE)) {
		std::cerr << "Erro ao inicializar Window" << std::endl;
		return false;
	}

	// Inicializar inputs
	InputHandler::initialize(window.getWindow());

	// Carregar GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("Falha ao inicializar GLAD");
    return false;
  }

	enableReportGlErrors();

  glEnable(GL_DEPTH_TEST);
  
  // Face Cull
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  
  // Para PNG
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glViewport(0, 0, WIDTH, HEIGHT);

	// Inicialize o player
	gPlayer = new Player(WIDTH, HEIGHT);

  loadAssets();
	
	scene.init();
	
	// Tudo certo
	gIsRunning = true;

	std::cout << "[OpenGL] OpenGL carregado com sucesso." << std::endl;

  spriteRenderer = new SpriteRenderer(ResourceManager::getInstance().getShader("ui"), WIDTH, HEIGHT);
	return true;
}

void Game::shutdown()
{
	delete gPlayer;
	gPlayer = nullptr;
}

const Terrain& Game::getTerrain() const { return *terrain; }

void Game::run()
{
	float deltaTime = 0.0f;
  float mLastFrame = 0.0f;
	
	while (gIsRunning && !window.windowShouldClose()) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - mLastFrame;
		mLastFrame = currentFrame;

		update(deltaTime);
		render();

		glfwPollEvents();
		window.swapBuffers();
	}
}

void Game::update(float deltaTime) 
{
  gPlayer->update(deltaTime, *terrain);
  scene.update(deltaTime);
  
  if (gPlayer) {
    glm::vec3 playerMin = gPlayer->getAABBMin();
    glm::vec3 playerMax = gPlayer->getAABBMax();

    const std::vector<GameObject*> sceneObjects = scene.getObjects();
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
            gPlayer->pCamera.position.x -= overlapX_depth;
          } else { // direita
            gPlayer->pCamera.position.x += overlapX_depth;
          }
          gPlayer->velocity.x = 0;
        } else if (overlapY_depth < overlapX_depth && overlapY_depth < overlapZ_depth) {
          if (playerMin.y < objMin.y) { // abaixo
            gPlayer->pCamera.position.y -= overlapY_depth;
          } else { // acima
            gPlayer->pCamera.position.y += overlapY_depth;
            gPlayer->isOnGround = true;
          }
          gPlayer->velocity.y = 0;
        } else {
          if (playerMin.z < objMin.z) { // frente
            gPlayer->pCamera.position.z -= overlapZ_depth;
          } else { // atras
            gPlayer->pCamera.position.z += overlapZ_depth;
          }
          gPlayer->velocity.z = 0;
        }
      }
    }
  }
}

void Game::render()
{
  renderer.render(*terrain, scene, gPlayer->getCamera());

  glDisable(GL_DEPTH_TEST);
  spriteRenderer->drawSprite(ResourceManager::getInstance().getTexture("crosshair"), glm::vec2((WIDTH - 32.0f) / 2.0f,
                                                                                               (HEIGHT - 32.0f) / 2.0f), glm::vec2(32.0f, 32.0f));
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
	resourceManager.loadModel("pendurador", "assets/models/pendurador/da p se pendurar.obj");
	resourceManager.loadModel("bike", "assets/models/bike/bike.obj");
	resourceManager.loadModel("caminhador", "assets/models/caminhador/caminhador.obj");
  resourceManager.loadModel("casa1", "assets/models/casa1/model.obj");
  resourceManager.loadModel("pig", "assets/models/pig/pig.obj");

  terrain = new Terrain("assets/heightmaps/heightmap.png", "assets/heightmaps/areia.jpg", 1025, 1025);

  // Sprites para a UI
  resourceManager.loadTexture("crosshair", "assets/sprites/crosshair.png");

  resourceManager.loadTexture("tree", "assets/sprites/tree.png");
}
