#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <string>
#include <glad/glad.h>
#include "stb_image.h"
#include "engine/graphics/Shader.hpp"

class Texture {
public: 
  GLuint ID;
  std::string type;
  GLuint unit;
  std::string path;

  Texture(const char* imagePath, const char* texType, GLuint slot, GLenum wrapping = GL_REPEAT);
  Texture();
  void Delete();

  void texUnit(Shader& shader, const char* uniform, GLuint uint);
  void bind() const;
  void unbind() const;

  int getWidth() const;
  int getHeight() const;

private:
  int width, height;
};

#endif
