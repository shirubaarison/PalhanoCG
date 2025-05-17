#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <iomanip>

#include "glm/ext/matrix_transform.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Terrain.hpp"
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
  Shader shader("assets/shaders/default.vs.glsl", "assets/shaders/default.fs.glsl");
  Shader skyboxShader("assets/shaders/skybox.vs.glsl", "assets/shaders/skybox.fs.glsl");
  Shader terrainShader("assets/shaders/terrain.vs.glsl", "assets/shaders/terrain.fs.glsl");

  // Camera
  Camera camera(WIDTH, HEIGHT, glm::vec3(3.0f, 5.0f, 5.0f));  

  // Modelo
  Model cat("assets/models/cat/cat_-_ps1_low_poly_rigged.obj");
  Model skeleton("assets/models/esqueleto/skeleton_character_psx.obj");

  // Skybox
  Skybox skybox("assets/skybox");
  skyboxShader.use();
  skyboxShader.setInt("skybox", 0);

  // Terreno
  Terrain terrain("assets/heightmaps/palhano_heightmap.png", 1000, 1000);
  terrain.loadTexture("assets/heightmaps/areia.jpg");

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  float angle = 0.0f;
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (camera.projectionType == ProjectionType::Perspective) {
      glDepthFunc(GL_LEQUAL);
      skyboxShader.use();
      glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.getViewMatrix()));
      skyboxShader.setMat4("view", skyboxView);
      skyboxShader.setMat4("projection", camera.getProjectionMatrix());
      skybox.render(skyboxShader);
      glDepthFunc(GL_LESS); 
    }

    shader.use();
    shader.setMat4("projection", camera.getProjectionMatrix());
    shader.setMat4("view", camera.getViewMatrix());

    if (angle == 360.0f)
      angle = 0.0f;
    angle += 3.0;
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(-6.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  
    cat.draw(shader, model);
    
    glm::mat4 sk_model = glm::mat4(1.0f);
    sk_model = glm::translate(sk_model, glm::vec3(4.0f, 3.0f, -1.0f));
    sk_model = glm::scale(sk_model, glm::vec3(5.0f, 5.0f, 5.0f));
    sk_model = glm::rotate(sk_model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    sk_model = glm::rotate(sk_model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  
    skeleton.draw(shader, sk_model);

    terrainShader.use();

    glm::mat4 terrainModel = glm::mat4(1.0f);
    terrainModel = glm::translate(terrainModel, glm::vec3(0.0f, -22.0f, 0.0f));
    terrainShader.setMat4("projection", camera.getProjectionMatrix());
    terrainShader.setMat4("view", camera.getViewMatrix());
    terrainShader.setMat4("model", terrainModel);

    terrain.draw(terrainShader);

    camera.inputs(window, deltaTime);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  shader.Delete();
  skyboxShader.Delete();
    
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
