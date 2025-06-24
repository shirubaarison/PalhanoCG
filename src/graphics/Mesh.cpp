#include "graphics/Mesh.hpp"
#include "graphics/Ebo.hpp"
#include <iostream>

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures, bool useTextures, const Material& material)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;
  Mesh::material = material;
  Mesh::useTextures = useTextures;

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
  // cores (location = 2)
  vao.linkAttrib(vbo, 2, 4, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, color));
  // coordenadas das texturas (location = 3) 
	vao.linkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	// Unbind em todos
	vao.unbind();
	vbo.unbind();
	ebo.unbind();
}

void Mesh::draw(Shader& shader, glm::mat4 model) 
{
  shader.use();
  vao.bind();
  
  if (useTextures) {
    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
      std::string num;
      std::string type = textures[i].type;
      if (type == "diffuse") {
        num = std::to_string(numDiffuse++);
      } else if (type == "specular") {
        num = std::to_string(numSpecular++);
      }
      textures[i].texUnit(shader, (type + num).c_str(), i);
      textures[i].bind();
    }

    glUniform1i(glGetUniformLocation(shader.ID, "useVertexColors"), 0);
  } else {
    glUniform1i(glGetUniformLocation(shader.ID, "useVertexColors"), 1);
  }

  GLint modelLoc = glGetUniformLocation(shader.ID, "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  
  // Material
  shader.setVec3("material.ambient", material.ambient);
  shader.setVec3("material.diffuse", material.diffuse);
  shader.setVec3("material.specular", material.specular);
  shader.setFloat("material.shininess", material.shininess);

  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); 
}

