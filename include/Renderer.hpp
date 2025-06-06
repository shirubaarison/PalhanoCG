#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "errorReporting.hpp"
#include "ResourceManager.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "errorReporting.hpp"

class Renderer {
public:
  Renderer();
  Renderer(Shader& shader);

  bool initialize(int width, int height);
  void render();
  void clear(const glm::vec4& clearColor);
  void shutdown();
private:
  Shader mShader;

  // Para iluminação direta (sol)
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(3.0f, 5.0f, 5.0f);
};

#endif