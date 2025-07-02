#ifndef GAME_OBJECT_CLASS_H
#define GAME_OBJECT_CLASS_H

#include "graphics/Shader.hpp"
#include "graphics/Model.hpp"
#include "glm/vec3.hpp"
#include <iostream>
#include <memory>

struct Transform {
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);  
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

class GameObject {
public:
  GameObject();
  GameObject(const std::string& name,
             const std::shared_ptr<Shader>& shader, 
             const std::shared_ptr<Model>& model, 
             const glm::vec3& pos,
             const glm::vec3& rot = glm::vec3(0, 0, 0),
             const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f));

  std::string name;
  bool isActive;

  Transform transform;

  std::shared_ptr<Shader> shader;     // shared ptr para compartilhar sem duplicar
  std::shared_ptr<Model> model;
};

#endif // !GAME_OBJECT_CLASS_H
