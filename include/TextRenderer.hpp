#ifndef TEXT_RENDERER_CLASS_H
#define TEXT_RENDERER_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <map>

#include "Shader.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H  

class TextRenderer {
public:
  TextRenderer(const std::string& fontPath);
  
  bool init(const std::string& fontPath, unsigned int fontSize);
  void renderText(Shader& shader, const std::string text, float x, float y, float scale, glm::vec3 color);

private:
  struct Character {
    GLuint textureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    long Advance;
  };

  std::map<char, Character> Characters;

  GLuint vao, vbo;
};

#endif
