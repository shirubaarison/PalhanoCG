#include "engine/core/Game.hpp"
#include "engine/core/Scene.hpp"
#include "engine/game/player/Camera.hpp"
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
    return false;
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

  delete player;
  player = nullptr;

  delete scene;
  scene = nullptr;
  
  delete ui;
  ui = nullptr;

  delete renderer;
  renderer = nullptr;

  delete input;
  input = nullptr;
  
  window->shutdown();
  delete window;
  window = nullptr;
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

		window->update();
	}
}

void Game::update(float deltaTime) 
{
  player->update(deltaTime, scene->getTerrain(), scene->getObjects());
  scene->update(deltaTime);
}

void Game::render()
{
  // Objetos 3D, terreno, billboard e skybox
  renderer->render(*scene, player->getCamera());

  // UI
  ui->render(player->pCamera.getProjectionType());
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
  ResourceManager::loadTexture("player", "assets/sprites/player.png");

  // Billboards
  ResourceManager::loadTexture("poste", "assets/sprites/poste.png");
  ResourceManager::loadTexture("mato1", "assets/sprites/mato1.png");
  ResourceManager::loadTexture("mato2", "assets/sprites/mato2.png");
  ResourceManager::loadTexture("mato3", "assets/sprites/mato3.png");
}
