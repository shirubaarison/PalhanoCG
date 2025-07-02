#include "core/Game.hpp"
#include "input/InputHandler.hpp"

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
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glViewport(0, 0, WIDTH, HEIGHT);

	// Inicialize o player
	gPlayer = new Player(WIDTH, HEIGHT);

  loadAssets();
	scene.init();
	
	// Tudo certo
	gIsRunning = true;

	std::cout << "[OpenGL] OpenGL carregado com sucesso." << std::endl;

	return true;
}

void Game::shutdown()
{
	delete gPlayer;
	gPlayer = nullptr;
}

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
  gPlayer->update(deltaTime);
  scene.update(deltaTime);
}

void Game::render()
{
  renderer.render(scene, gPlayer->getCamera());
}

void Game::loadAssets()
{
	// Shaders
	resourceManager.loadShader("default", "assets/shaders/default.vs.glsl", "assets/shaders/default.fs.glsl");

	// Models
  resourceManager.loadModel("saitama", "assets/models/saitama/saitama_ultimate_mode.obj");
  resourceManager.loadModel("pig", "assets/models/pig/pig.obj");
}
