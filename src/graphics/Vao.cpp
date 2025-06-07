#include "graphics/Vao.hpp"

VAO::VAO()
{
  glGenVertexArrays(1, &ID);
}

void VAO::Delete()
{
  glDeleteVertexArrays(1, &ID);
}

void VAO::linkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
  VBO.bind();
  glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
  glEnableVertexAttribArray(layout);
  VBO.unbind();
}

void VAO::bind() const
{
  glBindVertexArray(ID);
}

void VAO::unbind() const
{
  glBindVertexArray(0);
}
