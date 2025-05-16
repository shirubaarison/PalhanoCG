#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <iomanip>

#include "glm/ext/matrix_transform.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Terrain.hpp"
#include "TextRenderer.hpp"
#include "Skybox.hpp"

#define UNUSED(x) (void)(x) // só pra evitar avisos de variável não utilizada

// Configuração da janela
const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 720;
const char* TITLE = "TrabalhoCG";

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

int main() 
{
  if (!glfwInit()) {
    std::cerr << "Falha ao inicializar GLFW." << std::endl;
    return -1;
  }
  
  // OpenGL versão 3.3 Core profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  // duvido muito que alguem com mac vai rodar esse codigo mas blz
  #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif
  
  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
  if (window == NULL) {
    std::cerr << "Falha ao criar janela GLFW." << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  // Carregar GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Falha ao inicializar GLAD." << std::endl;
    return -1;
  }
  
  glViewport(0, 0, WIDTH, HEIGHT);
  glEnable(GL_DEPTH_TEST);
  
  // Build e compilar shaders  
  // Shader shader("assets/shaders/basic.vs.glsl", "assets/shaders/basic.fs.glsl");
  Shader shader("assets/shaders/default.vs.glsl", "assets/shaders/default.fs.glsl");

  // Camera
  Camera camera(WIDTH, HEIGHT, glm::vec3(3.0f, 3.0f, 5.0f));  

  // Modelo
  Model miku("assets/models/miku/miku_brazilian_fbx__rig(2).obj");

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));

    shader.setMat4("projection", camera.getProjectionMatrix());
    shader.setMat4("view", camera.getViewMatrix());
    shader.setMat4("model", model);

    shader.setVec3("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
    shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setVec3("viewPos", camera.getPosition());

    miku.draw(shader, model);

    camera.inputs(window);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  shader.Delete();
    
  glfwDestroyWindow(window);
  glfwTerminate();
  
  return 0;
}

// atualizar o tamanho da tela
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  // TODO: preservar aspect ratio
  UNUSED(window);
  glViewport(0, 0, width, height);
}
