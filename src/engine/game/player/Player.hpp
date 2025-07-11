#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H

#include "engine/game/world/Terrain.hpp"
#include "engine/game/player/Camera.hpp"
#include "engine/input/InputHandler.hpp"
#include <glm/glm.hpp>

class Player {
public:  
  Player(InputHandler* input);

  void update(float deltaTime, const Terrain& terrain);

  const Camera& getCamera() const;

  glm::vec3 velocity = glm::vec3(0.0f);
  glm::vec3 colliderSize = glm::vec3(1.0f, 2.0f, 1.0f);

  bool isAffectedByGravity = true;
  bool isOnGround = false;
  float jumpSpeed;
  bool jumpPressed;
  glm::vec3 getAABBMin() const;
  glm::vec3 getAABBMax() const;
  
  Camera pCamera;
private:
  InputHandler* input;
  float baseSpeed;
  float sprintMultiplier;
  
  float mouseSensitivity;
  bool firstClick;

  bool cameraTypeTogglePressed;

  void handleKeyboardInput(float deltaTime);
  void handleMouseInput();

  void applyPhysics(float deltaTime, const Terrain& terrain);
};

#endif // !PLAYER_CLASS_H

