#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/graphics/Renderer.hpp"
#include "engine/core/Scene.hpp"
#include "engine/graphics/SpriteRenderer.hpp"
#include "engine/utils/errorReporting.hpp"
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
  ResourceManager& resourceManager;
	Window window;

	Player *gPlayer;
  Renderer renderer;
  SpriteRenderer *spriteRenderer;
  Scene scene;

  Terrain* terrain;

	bool gIsRunning;

	void update(float deltaTime);
	void render();
};

#endif
