#include "InputHandler.hpp"
#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H

#include "Camera.hpp"
#include <glm/glm.hpp>

class Player {
public:  
  Player(int width, int height);

  void update(float deltaTime);

  const Camera& getCamera() const;
  
private:
  Camera pCamera;
  
  float baseSpeed;
  float sprintMultiplier;
  
  float mouseSensitivity;
  bool firstClick;

  bool cameraTypeTogglePressed;

  void handleKeyboardInput(float deltaTime);
  void handleMouseInput();
};

#endif // !PLAYER_CLASS_H

