#include "terrain.hpp"
#include "shader.hpp"

Terrain::Terrain(const std::string& path, int width, int length) 
{
  this->width = width;
  this->length = length;
  if (!loadHeightMap(path)) {
    std::cerr << "Deu erro no terreno pai" << std::endl;
  }
}

bool Terrain::loadHeightMap(const std::string& path)
{
  int width, height, nChannels;
  unsigned char *data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);

  if (!data) {
    std::cerr << "Falha ao ler HeightMap: " << path << std::endl;
    return false;
  }
  
  // popular vertices
  std::vector<float> vertices;
  std::vector<float> texCoords;
  float yScale = 64.0f / 256.0f, yShift = 16.0f;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      unsigned char *textel = data + (j + width * i) * nChannels;
      unsigned char y = textel[0];

      vertices.push_back(-height/2.0f + i);            // v.x
      vertices.push_back( (int) y * yScale - yShift);  // v.y (elevação da mesh)
      vertices.push_back(-width/2.0f + j);             // v.z
      
      texCoords.push_back(static_cast<float>(j) / (width - 1));   // u
      texCoords.push_back(static_cast<float>(i) / (height - 1));  // v
    }
  }

  stbi_image_free(data);

  // ebo
  std::vector<unsigned int> indices;
  for (int i = 0; i < height-1; i++) {
    for (int j = 0; j < width; j++) {
      for (int k = 0; k < 2; k++) {
        indices.push_back(j + width * (i + k));
      }
    }
  }

  // registrar VAO
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(float),
               &vertices[0],
               GL_STATIC_DRAW);

  // positions
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // texcoords
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               indices.size() * sizeof(unsigned int),
               &indices[0],
               GL_STATIC_DRAW);
  
  numStrips = height - 1;
  numVertPerStrip = width * 2;

  return true;
} 

void Terrain::loadTexture(const std::string& path)
{
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

  if (!data) {
    std::cerr << "Falha ao carregar textura para o terreno" << std::endl;
    return;
  }

  std::cout << "Texture loaded successfully: " 
              << width << "x" << height 
              << ", channels: " << nrChannels << std::endl; 

  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
}

void Terrain::draw(Shader& shader) 
{
  shader.activate();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glUniform1i(glGetUniformLocation(shader.ID, "terrainTex"), 0);

  // draw mesh
  glBindVertexArray(vao);
  // render 
  for(int strip = 0; strip < numStrips; ++strip) {
    glDrawElements(GL_TRIANGLE_STRIP,
                   numVertPerStrip,
                   GL_UNSIGNED_INT,
                   (void*)(sizeof(unsigned int) * numVertPerStrip * strip));
  }
}
