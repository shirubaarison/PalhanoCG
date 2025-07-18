#include "engine/core/GameObject.hpp"

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
  if (model) {
    objectType = ObjectType::MODEL; 
    colliderSize = model->aabbSize;
  }
  else
    objectType = ObjectType::BILLBOARD;
  
}

GameObject::GameObject() : isActive(true) { this->objectType = ObjectType::MODEL; }

void GameObject::updatePhysics(Terrain* terrain, float deltaTime) {}

void GameObject::draw(glm::mat4 modelMat) const 
{
  shader->setMat4("model", modelMat);
  
  if (model) {
    model->draw(*shader, modelMat);
  }
}

bool GameObject::checkAABBCollision(const GameObject& other) const
{
  glm::vec3 thisMin = transform.position - (colliderSize * transform.scale);
  glm::vec3 thisMax = transform.position + (colliderSize * transform.scale);

  glm::vec3 otherMin = other.transform.position - (other.colliderSize * other.transform.scale);
  glm::vec3 otherMax = other.transform.position + (other.colliderSize * other.transform.scale);

  bool overlapX = (thisMax.x > otherMin.x) && (thisMin.x < otherMax.x);
  bool overlapY = (thisMax.y > otherMin.y) && (thisMin.y < otherMax.y);
  bool overlapZ = (thisMax.z > otherMin.z) && (thisMin.z < otherMax.z);

  return overlapX && overlapY && overlapZ;
}

void GameObject::resolveCollision(GameObject& other)
{
  if (this->checkAABBCollision(other)) {
    if (this->affectedByGravity && !other.isStatic) {
      glm::vec3 penetration = glm::vec3(0.0f);

      if (this->transform.position.y > other.transform.position.y) {
        float thisBottom = this->transform.position.y - (this->colliderSize.y * this->transform.scale.y);
        float otherTop = other.transform.position.y + (other.colliderSize.y * other.transform.scale.y);

        if (thisBottom < otherTop) {
          penetration.y = otherTop - thisBottom;
        }
      }

      if (penetration.y > 0.0f) {
        this->transform.position.y += penetration.y;
        this->velocity.y = 0.0f; 
        this->isOnGround = true;
      }
    }
  }
}
