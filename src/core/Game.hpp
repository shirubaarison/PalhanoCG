#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/Renderer.hpp"
#include "core/Scene.hpp"
#include "utils/errorReporting.hpp"
#include "resources/ResourceManager.hpp"
#include "Window.hpp"
#include "player/Player.hpp"

enum GameState {
  GAME_ACTIVE,
  GAME_MENU
};

class Game {
public:
	Game();
  
  GameState state;
	bool initialize();
	void loadAssets();
	void run();
	void shutdown();
private:
  ResourceManager& resourceManager;
	Window window;

	Player *gPlayer;
  Renderer renderer;
  Scene scene;
	bool gIsRunning;

	void update(float deltaTime);
	void render();
};

#endif
