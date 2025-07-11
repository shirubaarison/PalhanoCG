#ifndef BILLBOARD_CLASS_H
#define BILLBOARD_CLASS_H

#include "core/GameObject.hpp" 
#include "graphics/Shader.hpp"
#include "graphics/Texture.hpp"
#include <glm/glm.hpp>

class Shader;
class Texture;

class Billboard : public GameObject {
public:
  Billboard();
  Billboard(Shader* billboardShader, Texture* billboardTexture,
            const glm::vec3& position = glm::vec3(0.0f),
            float scale = 1.0f,
            const glm::vec3& color = glm::vec3(1.0f),
            const std::string& name = "Billboard");

  ~Billboard();
  
  void draw() const override;

  void setTexture(Texture& texture) { this->texture = &texture; }
  void setScale(float s) { this->billboardScale = s; }
  void setColor(const glm::vec3& c) { this->billboardColor = c; }

  Texture* texture;

private:
  unsigned int quadVAO;
  float billboardScale;
  glm::vec3 billboardColor;
  void initRenderData();
};

#endif // !BILLBOARD_CLASS_H
