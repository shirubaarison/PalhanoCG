#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "errorReporting.hpp"
#include "ResourceManager.hpp"
#include "Window.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/common.hpp"
#include "Player.hpp"

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
