#ifndef TERRAIN_CLASS_H
#define TERRAIN_CLASS_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "engine/graphics/Shader.hpp"
#include "engine/graphics/Vao.hpp"
#include "engine/graphics/Texture.hpp"

class Terrain {
public:
  Terrain();
  Terrain(const std::string& path, const std::string& texturePath, int width, int length);
  void loadHeightMap(const std::string& path);
  void draw(Shader& shader) const;

  float getHeight(float worldX, float worldZ) const;

  Texture texture;
  VAO vao;
private:
  int width, length, numStrips, numVertPerStrip;
  std::vector<float> heights;
  std::vector<Vertex> vertices;
  std::vector<glm::vec3> normals;
  std::vector<unsigned int> indices;
};

#endif
