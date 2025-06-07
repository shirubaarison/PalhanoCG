#ifndef SKYBOX_CLASS_H
#define SKYBOX_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <stb_image.h>

#include "graphics/Shader.hpp"
#include "graphics/Vao.hpp"

class Skybox {
public:
  Skybox(const std::string& path);

  void load();
  void render(Shader& shader);

private:
  unsigned int loadCubemap(std::vector<std::string> faces);

  unsigned int cubemapTexture = 0;
  VAO vao;

  std::array<std::string, 6> facePaths;
};

#endif
