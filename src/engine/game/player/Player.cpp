#include "engine/game/player/Player.hpp"
#include "engine/core/GameObject.hpp"
#include "engine/game/player/Camera.hpp"
#include "engine/input/InputHandler.hpp"
#include "GLFW/glfw3.h"

Player::Player(InputHandler* input) 
  : jumpSpeed(4.0f),
  jumpPressed(false),
  pCamera(glm::vec3(0.0f, 1.0f, 5.0f)),
  input(input),
  baseSpeed(5.0f),
  sprintMultiplier(1.4f),
  mouseSensitivity(0.1f),
  firstClick(true),
  cameraTypeTogglePressed(false) {}

void Player::update(float deltaTime, const Terrain& terrain, std::vector<GameObject*> sceneObjects)
{
  handleKeyboardInput(deltaTime);
  handleMouseInput();
  applyPhysics(deltaTime, terrain);

  for (GameObject* obj : sceneObjects) {
    if (obj->isStatic)
      continue;

    doCollision(*obj);
  }
}

glm::vec3 Player::getAABBMin() const
{
  return pCamera.getPosition() - (colliderSize * 0.5f);
}

glm::vec3 Player::getAABBMax() const
{
  return pCamera.getPosition() + (colliderSize * 0.5f);
}

const Camera& Player::getCamera() const
{
  return pCamera;
}

void Player::handleKeyboardInput(float deltaTime) 
{
  float currentSpeed = baseSpeed;
  if (input->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
    currentSpeed *= sprintMultiplier;
  }
  float velocity = currentSpeed * deltaTime;
  
  glm::vec3 position = pCamera.getPosition();
  const glm::vec3 front = pCamera.getFront();
  const glm::vec3 right = pCamera.getRight(); 
  
  if (input->isKeyPressed(GLFW_KEY_W)) 
    position += front * velocity; 
  if (input->isKeyPressed(GLFW_KEY_S))
    position -= front * velocity;   
  if (input->isKeyPressed(GLFW_KEY_A))
    position -= right * velocity;
  if (input->isKeyPressed(GLFW_KEY_D))
    position += right * velocity;
  
  if (input->isKeyPressed(GLFW_KEY_SPACE) && !jumpPressed) {
    if (isOnGround) {
      this->velocity.y = jumpSpeed;   
      isOnGround = false;
    }
    jumpPressed = true;
  }
  if (input->isKeyReleased(GLFW_KEY_SPACE)) {
    jumpPressed = false;
  }
  
  pCamera.setPosition(position);
  
  if (input->isKeyPressed(GLFW_KEY_C) && !cameraTypeTogglePressed) {
    ProjectionType currentType = pCamera.getProjectionType();
    pCamera.setProjectionType(
      currentType == ProjectionType::Perspective ? ProjectionType::Orthographic : ProjectionType::Perspective
    );
    cameraTypeTogglePressed = true;
  }
  if (input->isKeyReleased(GLFW_KEY_C)) {
    cameraTypeTogglePressed = false;
  }
}

void Player::handleMouseInput() 
{
  input->setCursorMode(true);    
  if (firstClick) {
    input->centerCursor();
    firstClick = false;
  }
  
  glm::vec2 offset = input->getMouseOffset();
  
  if (pCamera.getProjectionType() == ProjectionType::Perspective) {
    float yawOffset = offset.x * mouseSensitivity;
    float pitchOffset = -offset.y * mouseSensitivity; 
    pCamera.processRotation(yawOffset, pitchOffset);
    pCamera.zoom = 45.0f;
  }
}

void Player::applyPhysics(float deltaTime, const Terrain& terrain)
{
  const glm::vec3 GRAVITY = glm::vec3(0.0f, -9.81f, 0.0f);
  isOnGround = false;
  
  velocity += GRAVITY * deltaTime;
  
  glm::vec3 currentPosition = pCamera.getPosition();
  currentPosition += velocity * deltaTime;
  
  // colisao de terreno
  float playerHalfHeight = colliderSize.y * 0.5f;
  float terrainHeightAtPlayerXZ = terrain.getHeight(currentPosition.x, currentPosition.z);
  
  if (currentPosition.y - playerHalfHeight <= terrainHeightAtPlayerXZ) {
    currentPosition.y = terrainHeightAtPlayerXZ + playerHalfHeight;
    if (velocity.y < 0) {
      velocity.y = 0.0f;
    }
    isOnGround = true;
  }
  
  pCamera.setPosition(currentPosition);
}

void Player::doCollision(const GameObject& obj)
{
  glm::vec3 playerMin = getAABBMin();
  glm::vec3 playerMax = getAABBMax();
  glm::vec3 objMin = obj.getAABBMin();
  glm::vec3 objMax = obj.getAABBMax();

  bool collisionX = (playerMax.x > objMin.x) && (playerMin.x < objMax.x);
  bool collisionY = (playerMax.y > objMin.y) && (playerMin.y < objMax.y);
  bool collisionZ = (playerMax.z > objMin.z) && (playerMin.z < objMax.z);

  // verificar colisão
  if (collisionX && collisionY && collisionZ) {
    float penetrationX = std::min(playerMax.x - objMin.x, objMax.x - playerMin.x);
    float penetrationY = std::min(playerMax.y - objMin.y, objMax.y - playerMin.y);
    float penetrationZ = std::min(playerMax.z - objMin.z, objMax.z - playerMin.z);

    glm::vec3 playerPos = pCamera.getPosition();

    if (penetrationX <= penetrationY && penetrationX <= penetrationZ) {
      // horizontalmente
      playerPos.x += (playerPos.x < obj.transform.position.x) ? -penetrationX : penetrationX;
      velocity.x = 0;
    }
    else if (penetrationY <= penetrationZ) {
      // verticalmente
      if (velocity.y <= 0 && playerPos.y > obj.transform.position.y) {
        // Player caindo 
        playerPos.y += penetrationY;
        isOnGround = true;
      } else {
        // para baixo ou para cima
        playerPos.y += (playerPos.y < obj.transform.position.y) ? -penetrationY : penetrationY;
      }
      velocity.y = 0;
    }
    else {
      playerPos.z += (playerPos.z < obj.transform.position.z) ? -penetrationZ : penetrationZ;
      velocity.z = 0;
    }

    pCamera.setPosition(playerPos);
  }
}
