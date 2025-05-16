#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include "Shader.hpp"
#include "glm/glm.hpp"
#include <vector>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

class VBO {
public:
  GLuint ID;
  VBO(const std::vector<Vertex>& vertices);
  void Delete();
  
  void bind() const;
  void unbind() const;
};

#endif
