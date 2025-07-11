#include "engine/graphics/Texture.hpp"
#include <iostream>

Texture::Texture(const char* imagePath, const char* texType, GLuint slot)
: type(texType), unit(slot), path(imagePath)
{
  // Carregar imagem com o stbi
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true); // tem que dar flip pq opengl carrega diferente da stbi
  unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);

  if (!data) {
    std::cerr << "Falha ao ler a textura: " << imagePath << std::endl;
    exit(1);
  }
  
  // Criar referência
  glGenTextures(1, &ID);
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, ID);

  // Parâmetros
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  // Determinar o formato, rgb ou rgba
  GLenum format = GL_RGB;
  if (nrChannels == 1)
    format = GL_RED;
  else if (nrChannels == 3)
    format = GL_RGB;
  else if (nrChannels == 4)
    format = GL_RGBA;

  // Gerar textura
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  this->height = height;
  this->width = width;

  if (data) {
    stbi_image_free(data);
  }

  std::cout << "[TEXTURE] Textura carregada com sucesso: " << imagePath 
    << " (" << width << "x" << height << ", " << nrChannels << " canais)" 
    << " Tipo: " << type << std::endl;
}

Texture::Texture() {}

int Texture::getWidth() const { return width; }
int Texture::getHeight() const { return height; }

void Texture::Delete()
{
  glDeleteTextures(1, &ID);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
  shader.use();
  glUniform1i(glGetUniformLocation(shader.ID, uniform), unit);
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, this->ID);
}

void Texture::bind() const
{
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind() const
{
  glBindTexture(GL_TEXTURE_2D, 0);
}
