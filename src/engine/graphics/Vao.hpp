#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include <engine/graphics/Vbo.hpp>

class VAO {
public:
  GLuint ID;
  VAO();
  void Delete();

  void linkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
  void bind() const;
  void unbind() const;
};

#endif
