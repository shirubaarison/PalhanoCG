#include "graphics/Shader.hpp"
#include "graphics/Vbo.hpp"

VBO::VBO(const std::vector<Vertex>& vertices)
{
  glGenBuffers(1, &ID);

  glBindBuffer(GL_ARRAY_BUFFER, ID);

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

VBO::VBO(const std::vector<float>& vertices)
{
  glGenBuffers(1, &ID);

  glBindBuffer(GL_ARRAY_BUFFER, ID);

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}

void VBO::Delete()
{
  glDeleteBuffers(1, &ID);
}

void VBO::bind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::unbind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
