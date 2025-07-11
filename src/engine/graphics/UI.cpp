#include "engine/graphics/UI.hpp"
#include "engine/resources/ResourceManager.hpp"
#include "engine/utils/Globals.hpp"

UI::UI()
{
  shader = &ResourceManager::getInstance().getShader("ui");
  
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Globals::WIDTH), static_cast<float>(Globals::HEIGHT), 0.0f, -1.0f, 1.0f);
  shader->use();
  shader->setInt("image", 0);
  shader->setMat4("projection", projection);

  initRenderData();
}

void UI::initRenderData()
{
  unsigned int VBO;
  float vertices[] = {
    // pos      // tex
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
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

void UI::drawSprite(Texture &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
  this->shader->use();
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(position, 0.0f));  

  model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
  model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
  model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

  model = glm::scale(model, glm::vec3(size, 1.0f)); 

  this->shader->setMat4("model", model);
  this->shader->setVec3("spriteColor", color);

  glActiveTexture(GL_TEXTURE0);
  texture.bind();

  glBindVertexArray(this->quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}
