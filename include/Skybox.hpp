#ifndef SKYBOX_CLASS_H
#define SKYBOX_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <array>
#include <vector>
#include <stb_image.h>

#include "Shader.hpp"

class Skybox {
public:
  Skybox(const std::string& path);

  void load();
  void render(Shader& shader);

private:
  unsigned int loadCubemap(std::vector<std::string> faces);

  unsigned int cubemapTexture = 0;
  GLuint vao, vbo;

  std::array<std::string, 6> facePaths;
};

#endif
