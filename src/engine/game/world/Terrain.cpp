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
      
      float repeatFactor = 2048.0f;
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
  float i_float = worldX + (this->length / 2.0f);   
  float j_float = worldZ + (this->width / 2.0f);   

  i_float = glm::clamp(i_float, 0.0f, (float)this->length - 1.0f);
  j_float = glm::clamp(j_float, 0.0f, (float)this->width - 1.0f);
  
  int i1 = static_cast<int>(i_float);
  int j1 = static_cast<int>(j_float);
  int i2 = glm::min(i1 + 1, this->length - 1);
  int j2 = glm::min(j1 + 1, this->width - 1);
  
  float fracI = i_float - i1;
  float fracJ = j_float - j1;
  
  float h11 = heights[i1 * this->width + j1];
  float h12 = heights[i1 * this->width + j2];
  float h21 = heights[i2 * this->width + j1];
  float h22 = heights[i2 * this->width + j2];
  
  float interpJ1 = glm::mix(h11, h12, fracJ);
  float interpJ2 = glm::mix(h21, h22, fracJ);
  float finalHeight = glm::mix(interpJ1, interpJ2, fracI);
  
  return finalHeight;
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
