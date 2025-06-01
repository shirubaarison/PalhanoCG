#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Shader.hpp"

// Dois tipos de camera, ortho e perpsectiva
enum class ProjectionType { Perspective, Orthographic };

class Camera {
public:
  // construtor
  Camera(int width, int height, glm::vec3 position);

  // envia a matriz de visualização * projeção para o shader
  void matrix(Shader& shader, const char* uniform);
  
  // trata da entrada do usuário
  void inputs(GLFWwindow* window, float deltaTime);
  
  glm::mat4 getProjectionMatrix() const;
  glm::mat4 getViewMatrix();
  glm::vec3 getPosition() const;

  bool useWireframe = false;
  bool useOrtho = false; // usar perspectiva por padrão
  ProjectionType projectionType = ProjectionType::Perspective;

  bool wireframePressed = false;
private:
  int width;
  int height;

  // vetores fundamentais da camera
  glm::vec3 position;
  glm::vec3 target;
  glm::vec3 right;
  glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 up;

  // matrizes calculadas
  glm::mat4 cameraMatrix = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  // evitar salto de click
  bool firstClick = true;
  
  // ângulos de rotação
  float yaw = -90.0f; // apontar para o -z negativo
  float pitch = 0.0f;
  
  // Velocidade de movimento
  float speed = 2.0f;
  const float sprintMultiplier = 4.0f;

  // sensibilidade e limitação do ângulo vertical
  const float sensitivity = 0.1f;   
  const float maxVerticalAngle = 89.0f; // evitar alinhamento de direction com up

  // Para a camera ortografica
  float rotationAngle = 0.0f;
  float rotationSpeed = 1.0f;
  float orthoZoomLevel = 10.0f;

  void processKeyboard(GLFWwindow* window, float deltaTime);
  void processMouse(GLFWwindow* window);

  void updateCameraVectors();
};

#endif
