#ifndef SPRITE_RENDERER_CLASS_H
#define SPRITE_RENDERER_CLASS_H

#include "engine/graphics/Shader.hpp"
#include "engine/graphics/Texture.hpp"
class SpriteRenderer {
public:
  SpriteRenderer(Shader &shader, int width, int height);
  ~SpriteRenderer();

  void drawSprite(Texture &texture, glm::vec2 position,
                  glm::vec2 size = glm::vec2(10.0f, 10.0f),
                  float rotate = 0.0f,
                  glm::vec3 color = glm::vec3(1.0f));
private:
  Shader shader;
  unsigned int quadVAO;
  int width, height;

  void initRenderData();
};

#endif // !SPRITE_RENDERER_CLASS_H
