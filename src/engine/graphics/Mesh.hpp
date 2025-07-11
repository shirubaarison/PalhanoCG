#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "engine/graphics/Vao.hpp"
#include "engine/graphics/Texture.hpp"
#include "engine/graphics/Shader.hpp"
#include "engine/graphics/Vbo.hpp"

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

	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures, bool useDiffuse, bool useSpecular, const Material& material);

	void draw(Shader& shader, glm::mat4 model);

private:
  bool useDiffuse;
  bool useSpecular;
};
#endif

