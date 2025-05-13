#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <iomanip>

#include "glm/ext/matrix_transform.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "terrain.hpp"
#include "textRenderer.hpp"
#include "skybox.hpp"

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
  Shader terrainShader("../shaders/terrain.vs.glsl", "../shaders/terrain.fs.glsl");
  Shader fontShader("../shaders/font.vs.glsl", "../shaders/font.fs.glsl");
  Shader skyboxShader("../shaders/skybox.vs.glsl", "../shaders/skybox.fs.glsl");

  // Carregar camera
  Camera camera(WIDTH, HEIGHT, glm::vec3(3.0f, 15.0f, 3.0f));

  // Carregar modelo
  Model miku("../assets/models/miku/miku_brazilian_fbx__rig(2).obj");
  
  Terrain terrain("../assets/heightmaps/iceland_heightmap.png", 2624, 1756);
  // terrain.loadTexture("../assets/heightmaps/tungtung.jpg");

  // para fontes
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

  TextRenderer textRenderer("../fonts/arial.ttf");
  fontShader.activate();
  glm::mat4 textProjection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT);
  fontShader.setMat4("projection", textProjection);

  // skybox
  Skybox skybox("../skybox");

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
  int frameCount = 0;
  float fpsCounterTime = 0.0f;
  float fps = 0.0f;
  
  float ang = 0.0f;
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    frameCount++;
    fpsCounterTime += deltaTime;

    if (fpsCounterTime >= 0.5f) {
      fps = frameCount / fpsCounterTime;
      frameCount = 0;
      fpsCounterTime = 0.0f;
    }

    camera.inputs(window, deltaTime);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100000.0f);
    glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.getViewMatrix()));

    skyboxShader.activate();
    skyboxShader.setMat4("projection", projection);
    skyboxShader.setMat4("view", skyboxView);
    skybox.render(skyboxShader);

    // Ativar classe de shader
    shader.activate();

    shader.setVec3("lightPos", glm::vec3(5.0f, 3.0f, 1.0f));
    shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    glm::mat4 view = camera.getViewMatrix();

    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setVec3("viewPos", camera.getPosition());

    if (ang == 360.0f) {
      ang = 0;
    }
    
    ang += 3.0f;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(ang), glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Desenhar o modelo
    miku.draw(shader, model);
    
    // terreno
    terrainShader.activate();

    glm::mat4 terrainModel = glm::mat4(1.0f);
    terrainModel = glm::translate(terrainModel, glm::vec3(0.0f, -14.0f, 0.0f));

    terrainShader.setMat4("projection", projection);
    terrainShader.setMat4("view", view);
    terrainShader.setMat4("model", terrainModel);
    
    terrain.draw(terrainShader);
    
    std::stringstream ss;
    ss << "FPS: " << std::fixed << std::setprecision(1) << fps;
    std::string fpsText = ss.str();
    
    textRenderer.renderText(fontShader, fpsText, 25.0f, HEIGHT - 50.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
    
       
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  shader.Delete();
  terrainShader.Delete();
    
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
