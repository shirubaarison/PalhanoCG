#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include "graphics/Shader.hpp"
#include <vector>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

class VBO {
public:
  GLuint ID;
  VBO(const std::vector<Vertex>& vertices);   // modelos
  VBO(const std::vector<float>& vertices);    // para skybox e terreno
  void Delete();
  
  void bind() const;
  void unbind() const;
};

#endif
