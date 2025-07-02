#include "GameObject.hpp"
#include <memory>

GameObject::GameObject() : isActive(true), transform(), shader(nullptr), model(nullptr) {}

GameObject::GameObject(const std::string& name,
                       const std::shared_ptr<Shader>& shader,
                       const std::shared_ptr<Model>& model,
                       const glm::vec3& pos,
                       const glm::vec3& rot,
                       const glm::vec3& scale) : 
  name(name), isActive(true), shader(shader), model(model)
{
  transform.position = pos;
  transform.rotation = rot;
  transform.scale = scale;
}
