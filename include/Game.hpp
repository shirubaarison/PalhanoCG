#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "errorReporting.hpp"
#include "ResourceManager.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/common.hpp"

class Game {
public:
	Game();

	bool initialize();
	void loadAssets();
	void run();
	void shutdown();
private:
	int width;
  int height;

  ResourceManager& resourceManager;
	Window window;
	Camera camera; 

	GLFWwindow *mWindow;

	bool mIsRunning;

	void processInput(float deltaTime);
	void update(float deltaTime);
	void render();
};

#endif
