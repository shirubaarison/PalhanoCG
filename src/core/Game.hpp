#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/errorReporting.hpp"
#include "resources/ResourceManager.hpp"
#include "Window.hpp"
#include "player/Player.hpp"

class Game {
public:
	Game();

	bool initialize();
	void loadAssets();
	void run();
	void shutdown();
private:
  ResourceManager& resourceManager;
	Window window;

	Player *gPlayer;

	bool gIsRunning;

	void update(float deltaTime);
	void render();
};

#endif
