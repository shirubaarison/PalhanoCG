#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "graphics/Vao.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Shader.hpp"

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

