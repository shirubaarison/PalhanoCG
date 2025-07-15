#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/graphics/Renderer.hpp"
#include "engine/core/Scene.hpp"
#include "engine/graphics/UI.hpp"
#include "engine/input/InputHandler.hpp"
#include "engine/resources/ResourceManager.hpp"
#include "engine/core/Window.hpp"
#include "engine/game/player/Player.hpp"
#include "engine/game/world/Terrain.hpp"

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

  const Terrain& getTerrain() const;
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
