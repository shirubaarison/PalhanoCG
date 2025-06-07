#include "Player.hpp"
#include "GLFW/glfw3.h"
#include <iostream>

Player::Player(int width, int height, GLFWwindow* window)
  : pInput(window),
  pCamera(width, height, glm::vec3(0.0f, 1.0f, 5.0f)),     // criar camera
  baseSpeed(5.0f),
  sprintMultiplier(2.0f),
  mouseSensitivity(0.1f),
  firstClick(true),
  cameraTypeTogglePressed(false) {}

void Player::update(float deltaTime)
{
  handleKeyboardInput(deltaTime);
  handleMouseInput();
}

const Camera& Player::getCamera() const
{
  return pCamera;
}

void Player::handleKeyboardInput(float deltaTime) 
{
  float currentSpeed = baseSpeed;
  if (pInput.isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
    currentSpeed *= sprintMultiplier;
  }

  float velocity = currentSpeed * deltaTime;

  glm::vec3 position = pCamera.getPosition();
  const glm::vec3 front = pCamera.getFront();
  const glm::vec3 right = pCamera.getRight(); 
  const glm::vec3 up = pCamera.getUp();

  if (pInput.isKeyPressed(GLFW_KEY_W)) 
    position += front * velocity; 
  if (pInput.isKeyPressed(GLFW_KEY_S))
    position -= front * velocity;
  if (pInput.isKeyPressed(GLFW_KEY_A))
    position -= right * velocity;
  if (pInput.isKeyPressed(GLFW_KEY_D))
    position += right * velocity;
  if (pInput.isKeyPressed(GLFW_KEY_SPACE))
    position += up * velocity;
  if (pInput.isKeyPressed(GLFW_KEY_LEFT_SHIFT))
    position -= up * velocity;

  pCamera.setPosition(position);

  // Trocar de camera
  if (pInput.isKeyPressed(GLFW_KEY_C) && !cameraTypeTogglePressed) {
    ProjectionType currentType = pCamera.getProjectionType();
    pCamera.setProjectionType(
      currentType == ProjectionType::Perspective ? ProjectionType::Orthographic : ProjectionType::Perspective
    );
    cameraTypeTogglePressed = true;
  }
  if (pInput.isKeyReleased(GLFW_KEY_C)) {
    cameraTypeTogglePressed = false;
  }
}

void Player::handleMouseInput() 
{
  if (pInput.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
    pInput.setCursorMode(true); 

    if (firstClick) {
      pInput.centerCursor();
      firstClick = false;
    }

    glm::vec2 offset = pInput.getMouseOffset();

    if (pCamera.getProjectionType() == ProjectionType::Perspective) {
      float yawOffset = offset.x * mouseSensitivity;
      float pitchOffset = -offset.y * mouseSensitivity; 
      pCamera.processRotation(yawOffset, pitchOffset);
    } else {
      pCamera.processZoom(offset.y * 0.05f);
    }
  } else {
    pInput.setCursorMode(false);  
    firstClick = true;
  }
}

