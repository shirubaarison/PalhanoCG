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
const char* TITLE = "Untitled Game";

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
  Shader shader("../shaders/basic.vs.glsl", "../shaders/basic.fs.glsl");

  // Carregar camera
  Camera camera(WIDTH, HEIGHT, glm::vec3(3.0f, 15.0f, 3.0f));

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100000.0f);

    shader.activate();

    glm::mat4 view = camera.getViewMatrix();

    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setVec3("viewPos", camera.getPosition());

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
       
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
