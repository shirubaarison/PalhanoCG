#ifndef GAME_OBJECT_CLASS_H
#define GAME_OBJECT_CLASS_H

#include "engine/graphics/Shader.hpp"
#include "engine/graphics/Model.hpp"
#include "glm/vec3.hpp"
#include <iostream>

struct Transform {
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);  
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

enum ObjectType {
  MODEL,
  BILLBOARD
};

class GameObject {
public:
  GameObject();
  GameObject(const std::string& name,
             Shader* shader, 
             Model* model, 
             const glm::vec3& pos,
             const glm::vec3& rot = glm::vec3(0, 0, 0),
             const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f));
  
  std::string name;
  ObjectType objectType;
  bool isActive;

  Transform transform;

  Shader* shader;
  Model* model;

  // fisica
  glm::vec3 velocity = glm::vec3(0.0f);
  glm::vec3 colliderSize = glm::vec3(1.0f); // colisao AABB

  bool isStatic = false;
  bool affectedByGravity = false;
  bool isOnGround = false;

  void updatePhysics(float deltaTime);
  bool checkAABBCollision(const GameObject& other) const;
  void resolveCollision(GameObject& other);

  virtual void draw() const;
};

#endif // !GAME_OBJECT_CLASS_H
