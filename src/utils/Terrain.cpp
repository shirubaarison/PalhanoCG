#include "utils/Terrain.hpp"
#include "graphics/Ebo.hpp"
#include <iostream>

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
  }
  
  // VBO
  std::vector<float> vertices;
  float yScale = 64.0f / 256.0f, yShift = 16.0f;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      unsigned char *textel = data + (j + width * i) * nChannels;
      unsigned char y = textel[0]; // valor r do rgb, tanto faz aqui já que é

      vertices.push_back(-height/2.0f + i);            // v.x
      vertices.push_back( (int) y * yScale - yShift);  // v.y (elevação da mesh)
      vertices.push_back(-width/2.0f + j);             // v.z
      
      float repeatFactor = 1024.0f;
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

void Terrain::draw(Shader& shader) 
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
