#include "engine/game/world/Terrain.hpp"
#include "engine/graphics/Ebo.hpp"
#include <iostream>

Terrain::Terrain() {}

Terrain::Terrain(const std::string& path, const std::string& texturePath, int width, int length) 
{
  this->width = width;
  this->length = length;
  
  loadHeightMap(path);
  this->texture = Texture(texturePath.c_str(), "diffuse", 0);
}

void Terrain::loadHeightMap(const std::string& path)
{
  int width, height, nChannels;
  unsigned char *data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);
  if (!data) {
    std::cerr << "Falha ao ler HeightMap: " << path << std::endl;
    return;
  }
  
  heights.resize(width * height);
  float yScale = 64.0f / 256.0f, yShift = 3.0f;
  
  // VBO
  std::vector<float> vertices;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      unsigned char *textel = data + (j + width * i) * nChannels;
      unsigned char y = textel[0]; // valor r do rgb, tanto faz aqui já que é
      
      float heightValue = (int) y * yScale - yShift;
      heights[i * width + j] = heightValue;
      
      vertices.push_back(-height/2.0f + i);            // v.x
      vertices.push_back(heightValue);                 // v.y (elevação da mesh)
      vertices.push_back(-width/2.0f + j);             // v.z
      
      float repeatFactor = 512.0f;
      vertices.push_back(j * repeatFactor / (width - 1));  // u
      vertices.push_back(i * repeatFactor / (height - 1)); // v
    }
  }
  stbi_image_free(data);
  
  // EBO
  std::vector<GLuint> indices;
  for (int i = 0; i < height-1; i++) {
    for (int j = 0; j < width; j++) {
      for (int k = 0; k < 2; k++) {
        indices.push_back(j + width * (i + k));
      }
    }
  }
  
  // registrar VAO
  vao.bind();
  VBO vbo(vertices);
  EBO ebo(indices);
  // Posições
  vao.linkAttrib(vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
  // TexCoords
  vao.linkAttrib(vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  numStrips = height - 1;
  numVertPerStrip = width * 2;
  vao.unbind();
  vbo.unbind();
  ebo.unbind();
} 


float Terrain::getHeight(float worldX, float worldZ) const 
{
  float x = worldX + (length / 2.0f);   
  float z = worldZ + (width / 2.0f);   
  
  x = glm::clamp(x, 0.0f, (float)length - 1.0f);
  z = glm::clamp(z, 0.0f, (float)width - 1.0f);
  
  int x1 = (int)x, z1 = (int)z;
  int x2 = glm::min(x1 + 1, length - 1);
  int z2 = glm::min(z1 + 1, width - 1);
  
  float fx = x - x1;
  float fz = z - z1;
  
  float h1 = heights[x1 * width + z1];  // inferior-esquerdo
  float h2 = heights[x1 * width + z2];  // inferior-direito
  float h3 = heights[x2 * width + z1];  // superior-esquerdo
  float h4 = heights[x2 * width + z2];  // superior-direito
  
  // interpola 
  float linha1 = glm::mix(h1, h2, fz);  
  float linha2 = glm::mix(h3, h4, fz); 
  return glm::mix(linha1, linha2, fx);
}

void Terrain::draw(Shader& shader) const
{
  shader.use();  
  vao.bind();
  texture.bind();
  
  for(int strip = 0; strip < numStrips; ++strip) {
    glDrawElements(GL_TRIANGLE_STRIP,
                   numVertPerStrip,
                   GL_UNSIGNED_INT,
                   (void*)(sizeof(unsigned int) * numVertPerStrip * strip));
  }
}
