#include "Renderer.hpp"
#include <iostream>

Renderer::Renderer() {}

Renderer::Renderer(Shader& shader) 
{
  mShader = shader;
}

bool Renderer::initialize(int width, int height)
{
  // Load GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("Failed to initialize GLAD");
    return false;
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  enableReportGlErrors();

  glViewport(0, 0, width, height);
  
  return true;
}

void Renderer::clear(const glm::vec4& clearColor) {
  glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::shutdown()
{
  
}