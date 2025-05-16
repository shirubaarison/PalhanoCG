#ifndef TERRAIN_CLASS_H
#define TERRAIN_CLASS_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "Vbo.hpp"
#include "Shader.hpp"

class Terrain {
public:
  Terrain(const std::string& path, int width, int length);
  bool loadHeightMap(const std::string& path);
  void loadTexture(const std::string& texturePath);
  void draw(Shader& shader);

  GLuint vao, vbo, ebo, textureID;
private:
  int width, length, numStrips, numVertPerStrip;
  std::vector<float> heights;
  std::vector<Vertex> vertices;
  std::vector<glm::vec3> normals;
  std::vector<unsigned int> indices;
};

#endif
