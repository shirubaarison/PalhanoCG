#include "core/Game.hpp"
#include "glm/trigonometric.hpp"
#include "input/InputHandler.hpp"

#include <iostream>
#include <glm/glm.hpp>

unsigned int WIDTH = 1280;
unsigned int HEIGHT = 720;
const char* TITLE = "Trabalho CG";

Game::Game() 
  : resourceManager(ResourceManager::getInstance()),
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
}

void Game::render()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Shader shader = resourceManager.getShader("default");
	
	shader.use();
	shader.setMat4("projection", gPlayer->getCamera().getProjectionMatrix());
	shader.setMat4("view", gPlayer->getCamera().getViewMatrix());
	shader.setVec4("lightColor",  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	shader.setVec3("lightPos",  glm::vec3(0.0f, 5.0f, 5.0f));
	shader.setVec3("camPos", gPlayer->getCamera().getPosition());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
  model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	Model cat = resourceManager.getModel("saitama");
	cat.draw(shader, model);
}

void Game::loadAssets()
{
	// Shaders
	resourceManager.loadShader("default", "assets/shaders/default.vs.glsl", "assets/shaders/default.fs.glsl");

	// Models
	// resourceManager.loadModel("cat", "assets/models/cat/cat_-_ps1_low_poly_rigged.obj");
	//  resourceManager.loadModel("tree", "assets/models/tree/model.obj");
	//  resourceManager.loadModel("gtr", "assets/models/gtr/Nissan GTR.obj");
	//  resourceManager.loadModel("pig", "assets/models/pig/pig.obj");
	//  resourceManager.loadModel("su", "assets/models/su/russian_su-25.obj");
	//  resourceManager.loadModel("bike", "assets/models/bike/bike.obj");
  resourceManager.loadModel("saitama", "assets/models/saitama/saitama_ultimate_mode.obj");
}
