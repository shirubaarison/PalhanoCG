#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "shader.hpp"

class Camera {
public:
  Camera(int width, int height, glm::vec3 position);

  void matrix(Shader& shader, const char* uniform);
  void inputs(GLFWwindow* window, float deltaTime);
  
  glm::mat4 getProjectionMatrix() const;
  glm::mat4 getViewMatrix();
  glm::vec3 getPosition() const;

private:
  int width;
  int height;
  float deltaTime;

  glm::vec3 position;
  glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

  glm::mat4 cameraMatrix = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  bool firstClick = true;
  
  float yaw = -90.0f;
  float pitch = 0.0f;
  float speed = 5.0f;
  
  float baseSpeed = 1.0f;
  const float sprintMultiplier = 4.0f;
  const float sensitivity = 0.1f;   
  const float maxVerticalAngle = 89.0f;

  void processKeyboard(GLFWwindow* window);
  void processMouse(GLFWwindow* window);
};

#endif
