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
  Camera(int width, int height, glm::vec3 position);

  // envia a matriz de visualização * projeção para o shader
  void matrix(Shader& shader, const char* uniform);
  
  // trata da entrada do usuário
  void inputs(GLFWwindow* window, float deltaTime);
  
  glm::mat4 getProjectionMatrix() const;
  glm::mat4 getViewMatrix();
  glm::vec3 getPosition() const;
private:
  int width;
  int height;

  // vetores fundamentais da camera
  glm::vec3 position;
  glm::vec3 target;
  glm::vec3 right;
  glm::vec3 up;
  glm::vec3 worldUp;

  // matrizes calculadas
  glm::mat4 cameraMatrix;
  glm::mat4 projection;

  bool useWireframe;
  bool useOrtho;
  ProjectionType projectionType;

  bool wireframePressed;
  // evitar salto de click
  bool firstClick = true;
  
  // ângulos de rotação
  float yaw; 
  float pitch;
  
  // Velocidade de movimento
  float speed;
  float sprintMultiplier;

  // sensibilidade e limitação do ângulo vertical
  float sensitivity;   
  float maxVerticalAngle; // evitar alinhamento de direction com up

  // Para a camera ortografica
  float rotationAngle;
  float rotationSpeed;
  float orthoZoomLevel;

  void processKeyboard(GLFWwindow* window, float deltaTime);
  void processMouse(GLFWwindow* window);

  void updateCameraVectors();
};

#endif
