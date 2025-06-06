#include "Game.hpp"

unsigned int WIDTH = 1280;
unsigned int HEIGHT = 720;
const char* TITLE = "Trabalho CG";

Game::Game() 
  : width(1280),
    height(720),
    resourceManager(ResourceManager::getInstance()),
    window(), 
		camera(WIDTH, HEIGHT, glm::vec3(0.0f, 3.0f, 0.0f)),
    mIsRunning(false) {}

bool Game::initialize()
{
	// Inicializar janelas
	if (!window.initialize(WIDTH, HEIGHT, TITLE)) {
		std::cerr << "Erro ao inicializar WindowManager" << std::endl;
		return false;
	}

	mWindow = window.getWindow();
	
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
	
	// Tudo certo
	mIsRunning = true;

	std::cout << "[OpenGL] OpenGL carregado com sucesso." << std::endl;

	return true;
}

void Game::shutdown()
{
	
}

void Game::run()
{
	float deltaTime = 0.0f;
  float mLastFrame = 0.0f;
	
	while (mIsRunning && !window.windowShouldClose()) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - mLastFrame;
		mLastFrame = currentFrame;

		processInput(deltaTime);

		update(deltaTime);

		render();

		glfwPollEvents();
		window.swapBuffers();
	}
}

void Game::processInput(float deltaTime)
{
	camera.inputs(mWindow, deltaTime);

	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(mWindow, true);

	if (glfwGetKey(mWindow, GLFW_KEY_F) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Game::update(float deltaTime) 
{
	
}

void Game::render()
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Shader shader = resourceManager.getShader("default");
	
	shader.use();
	shader.setMat4("projection", camera.getProjectionMatrix());
	shader.setMat4("view", camera.getViewMatrix());
	shader.setVec4("lightColor",  glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	shader.setVec3("lightPos",  glm::vec3(2.0f, 5.0f, 2.0f));
	shader.setVec3("camPos", camera.getPosition());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));

	Model cat = resourceManager.getModel("cat");
	cat.draw(shader, model);
}

void Game::loadAssets()
{
	// Shaders
	resourceManager.loadShader("default", "../assets/shaders/default.vs.glsl", "../assets/shaders/default.fs.glsl");

	// Models
	// resourceManager.loadModel("backpack", "assets/models/backpack/backpack.obj");
	resourceManager.loadModel("cat", "../assets/models/cat/cat_-_ps1_low_poly_rigged.obj");
}
