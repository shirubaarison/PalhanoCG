#include "Mesh.hpp"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	vao.bind();
	// Gerar VBO e linkar os vertices
	VBO vbo(vertices);
	// Gerar EBO e linkar os indices
	EBO ebo(indices);

	// Linkar VBO
	// position (location = 0)
	vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
	// normais (location = 1)
	vao.linkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// coordenadas das texturas (location = 2) 
	vao.linkAttrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	// Unbind em todos
	vao.unbind();
	vbo.unbind();
	ebo.unbind();
}


void Mesh::draw(Shader& shader, glm::mat4 model) 
{
	shader.use();
	vao.bind();

  unsigned int numDiffuse = 0;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse") {
			num = std::to_string(numDiffuse++);
		} 
    textures[i].texUnit(shader, (type + num).c_str(), i);
    textures[i].bind();
  }

  GLint modelLoc = glGetUniformLocation(shader.ID, "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

  // Desenha
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); 
}

