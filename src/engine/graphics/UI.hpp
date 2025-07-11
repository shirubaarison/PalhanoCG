#ifndef UI_CLASS
#define UI_CLASS

#include "engine/graphics/Shader.hpp"
#include "engine/graphics/Texture.hpp"

class UI {
public:
  UI(Shader &shader, int width, int height);
  ~UI();

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

#endif // !UI_CLASS
