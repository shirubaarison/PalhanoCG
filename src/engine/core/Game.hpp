#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/graphics/Renderer.hpp"
#include "engine/core/Scene.hpp"
#include "engine/graphics/UI.hpp"
#include "engine/input/InputHandler.hpp"
#include "engine/core/Window.hpp"
#include "engine/game/player/Player.hpp"

class Game {
public:
	Game();
  
	bool initialize();
	void loadAssets();
	void run();
	void shutdown();
private:
  Window *window;
  InputHandler *input;
	Player *player;
  Renderer *renderer;
  UI *ui;
  Scene *scene;

	void update(float deltaTime);
	void render();
};

#endif
