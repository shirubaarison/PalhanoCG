#include "engine/core/GameObject.hpp"
#include <cstddef>

GameObject::GameObject(const std::string& name,
                       Shader* shader,
                       Model* model,
                       const glm::vec3& pos,
                       const glm::vec3& rot,
                       const glm::vec3& scale) : 
  name(name), isActive(true), shader(shader), model(model)
{
  transform.position = pos;
  transform.rotation = rot;
  transform.scale = scale;

  // se tiver modelo, é 3D se não billboard
  if (model != NULL) {
    objectType = ObjectType::MODEL;
    colliderSize = model->aabbSize;
  }
  else
    objectType = ObjectType::BILLBOARD;
  
}

GameObject::GameObject() : isActive(true) { this->objectType = ObjectType::MODEL; }

void GameObject::draw(glm::mat4 modelMat) const 
{
  shader->setMat4("model", modelMat);
  
  if (model) {
    model->draw(*shader, modelMat);
  }
}

AABB GameObject::getTransformedAABB() const
{
  // Billboard
  if (!model) {
    glm::vec3 halfSize = (colliderSize * transform.scale) * 0.5f;
    glm::vec3 min = transform.position - halfSize;
    glm::vec3 max = transform.position + halfSize;
    return { min, max };
  }
  
  // 3D
  glm::vec3 modelCenter = model->aabbCenter;
  glm::vec3 halfSize = (colliderSize * transform.scale) * 0.5f;
  glm::vec3 worldCenter = transform.position + (modelCenter * transform.scale);

  glm::vec3 min = worldCenter - halfSize;
  glm::vec3 max = worldCenter + halfSize;  

  return { min, max };
}

glm::vec3 GameObject::getAABBMin() const 
{
  return getTransformedAABB().min;
}

glm::vec3 GameObject::getAABBMax() const 
{
  return getTransformedAABB().max;
}
