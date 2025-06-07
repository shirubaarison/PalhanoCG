#include "graphics/Ebo.hpp"

EBO::EBO(std::vector<GLuint>& indices)
{
  glGenBuffers(1, &ID);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void EBO::Delete()
{
  glDeleteBuffers(1, &ID);
}

void EBO::bind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::unbind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
