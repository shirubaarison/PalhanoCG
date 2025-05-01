#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "model.hpp"
#include "camera.hpp"
#include "shader.hpp"

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
  Shader shader("../shaders/default.vs.glsl", "../shaders/default.fs.glsl");

  // Carregar camera
  Camera camera(WIDTH, HEIGHT, glm::vec3(3.0f, 2.0f, 3.0f));

  // Carregar modelo
  Model ourModel("../assets/models/van/psx_low-poly_camper_van.obj");
  Model guitar("../assets/models/guitar/psx_low-poly_acoustic_guitar.obj");
  Model trainer("../assets/models/trainer/elliptical_trainer.obj");

  float ang = 0.0f;
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    
    camera.inputs(window);
    camera.updateMatrix(45.0f, 0.1f, 100.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Ativar classe de shader
    shader.activate();

    shader.setVec3("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
    shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.getViewMatrix();

    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setVec3("viewPos", camera.getPosition());

    camera.matrix(shader, "cameraMatrix");
    
    if (ang == 360.0f) {
      ang = 0;
    }
    
    ang += 1.0f;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
    // model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // model = glm::rotate(model, glm::radians(ang), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

    // Desenhar o modelo
    ourModel.draw(shader, model);

    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f)); 
    guitar.draw(shader, model);


    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f)); 
    trainer.draw(shader, model);
    
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
  UNUSED(window);
  glViewport(0, 0, width, height);
}
