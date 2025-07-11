#include "engine/graphics/Billboard.hpp" 
#include <glad/glad.h>  
#include <glm/gtc/matrix_transform.hpp> 

Billboard::Billboard() {}

Billboard::Billboard(Shader* billboardShader, Texture* billboardTexture,
                     const glm::vec3& position, float scale,
                     const glm::vec3& color, const std::string& name)
  : GameObject(name, billboardShader, nullptr, position, glm::vec3(0.0f), glm::vec3(scale)),
  texture(billboardTexture),
  billboardScale(scale),              
  billboardColor(color)
{
  this->transform.scale = glm::vec3(scale); 
  initRenderData(); 
}

Billboard::~Billboard() {
  glDeleteVertexArrays(1, &this->quadVAO);
}

void Billboard::initRenderData() {
  unsigned int VBO;
  float vertices[] = {
    -0.5f, -0.5f,       0.0f, 0.0f, 
    0.5f, -0.5f,       1.0f, 0.0f, 
    -0.5f,  0.5f,       0.0f, 1.0f, 

    -0.5f,  0.5f,       0.0f, 1.0f, 
    0.5f, -0.5f,       1.0f, 0.0f,
    0.5f,  0.5f,       1.0f, 1.0f
  };

  glGenVertexArrays(1, &this->quadVAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(this->quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Billboard::draw() const {
  shader->setFloat("billboardScale", this->billboardScale);
  shader->setVec3("spriteColor", this->billboardColor);

  if (texture) {
    glActiveTexture(GL_TEXTURE0);
    texture->bind();
    shader->setInt("image", 0); 
  }

  glBindVertexArray(this->quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}
