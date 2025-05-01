#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>
#include <vector>
#include <unordered_set>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cgltf.h>

#include "vao.hpp"
#include "ebo.hpp"
#include "texture.hpp"
#include "shader.hpp"

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;
	// Store VAO in public so it can be used in the Draw function
	VAO vao;

	// Initializes the mesh
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

	// Draws the mesh
	void draw(Shader& shader, glm::mat4 model);
};
#endif

