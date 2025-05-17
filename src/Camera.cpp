#include "Camera.hpp"
#include "GLFW/glfw3.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
  this->width = width;
  this->height = height;
  this->position = position;
  
  // inicializar front, right e up
  updateCameraVectors();
}

void Camera::matrix(Shader& shader, const char* uniform)
{
  glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

glm::mat4 Camera::getViewMatrix()
{
  glm::mat4 view = glm::lookAt(position, position + front, up); 
  
  if (projectionType == ProjectionType::Orthographic) {
    // arrumar isso depois
    view = glm::lookAt(glm::vec3(10.0f, 10.0f, 15.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, -position);
    view = glm::rotate(view, glm::radians(rotationAngle), glm::vec3(0, 1, 0));
  }

  return view;
}

void Camera::inputs(GLFWwindow* window, float deltaTime)
{
  processKeyboard(window, deltaTime);
  processMouse(window);
}

void Camera::processKeyboard(GLFWwindow* window, float deltaTime)
{
  // Perspectiva
  if (projectionType == ProjectionType::Perspective) {
    float currentSpeed = speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
      currentSpeed *= sprintMultiplier;

    // Frente e atrás
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      position += currentSpeed * front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      position -= currentSpeed * front;

    // Esquerda e direita
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      position -= currentSpeed * right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      position += currentSpeed * right;

    // Voar ou ir pra baixo
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      position += currentSpeed * up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      position -= currentSpeed * up;
  }

  // Ortho
  if (projectionType == ProjectionType::Orthographic) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
      rotationAngle += rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
      rotationAngle -= rotationSpeed;
  }

  // Trocar tipo de câmera
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !useOrtho) {
    useOrtho = true;
    if (projectionType == ProjectionType::Perspective)
      projectionType = ProjectionType::Orthographic;
    else
      projectionType = ProjectionType::Perspective;
  }
  
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
    useOrtho = false;

  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !wireframePressed)
  {
    wireframePressed = true;
    useWireframe = !useWireframe;
  }

  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
    wireframePressed = false;
  
  if (useWireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Camera::processMouse(GLFWwindow* window)
{
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    // Desabilitar cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (firstClick) {
      glfwSetCursorPos(window, (float)width / 2, (float)height / 2); // Colocar no meio
      firstClick = false;
    }

    if (projectionType == ProjectionType::Perspective)
    {
      double mouseX, mouseY;
      glfwGetCursorPos(window, &mouseX, &mouseY);

      float offsetX = static_cast<float>(mouseX - (float)width / 2);
      float offsetY = static_cast<float>(mouseY - (float)height / 2);

      yaw += offsetX * sensitivity;
      pitch -= offsetY * sensitivity;  

      if (pitch > maxVerticalAngle) pitch = maxVerticalAngle;
      if (pitch < -maxVerticalAngle) pitch = -maxVerticalAngle;

      glm::vec3 newOrientation;
      newOrientation.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
      newOrientation.y = sin(glm::radians(pitch));
      newOrientation.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

      front = glm::normalize(newOrientation);
    }
    glfwSetCursorPos(window, (float)width / 2, (float)height / 2);
  } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // volta o cursor ao normal
    firstClick = true;
  }

  updateCameraVectors();
}

void Camera::updateCameraVectors()
{
  // calcular nova direção com base no yaw e pitch
  glm::vec3 front_v;
  front_v.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front_v.y = sin(glm::radians(pitch));
  front_v.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front_v = glm::normalize(front_v);

  // atualizar vetores ortogonais
  right = glm::normalize(glm::cross(front, worldUp)); 
  up = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::getProjectionMatrix() const 
{ 
  if (projectionType == ProjectionType::Perspective)
    return glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 1000.0f);
  else
    return glm::ortho(-10.0f, 10.0f, -10.0f * height / width, 10.0f * height / width, 0.1f, 1000.0f);
}
glm::vec3 Camera::getPosition() const { return position; }
