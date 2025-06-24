#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "graphics/Vao.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Shader.hpp"
#include "graphics/Vbo.hpp"

struct Material {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
};

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

  Material material;

	VAO vao;

	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures, bool useTextures, const Material& material);

	void draw(Shader& shader, glm::mat4 model);

private:
  bool useTextures;
};
#endif

