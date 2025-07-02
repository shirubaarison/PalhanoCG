#ifndef GAME_OBJECT_CLASS_H
#define GAME_OBJECT_CLASS_H

#include "graphics/Shader.hpp"
#include "graphics/Model.hpp"
#include "glm/vec3.hpp"
#include <iostream>

struct Transform {
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);  
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
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
  bool isActive;

  Transform transform;

  Shader* shader;
  Model* model;
};

#endif // !GAME_OBJECT_CLASS_H
