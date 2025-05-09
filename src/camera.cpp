#include "camera.hpp"
#include "GLFW/glfw3.h"

Camera::Camera(int width, int height, glm::vec3 position)
    : width(width), height(height), position(position) {}

void Camera::matrix(Shader& shader, const char* uniform)
{
  glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

glm::mat4 Camera::getViewMatrix()
{
  return glm::lookAt(position, position + orientation, up);
}

void Camera::inputs(GLFWwindow* window, float deltaTime)
{
  this->deltaTime = deltaTime;
  processKeyboard(window);
  processMouse(window);
}

void Camera::processKeyboard(GLFWwindow* window)
{
  const glm::vec3 right = glm::normalize(glm::cross(orientation, up));

  const glm::vec3 actualUp = glm::normalize(glm::cross(right, orientation));

  float currentSpeed = speed * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    currentSpeed *= sprintMultiplier;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    position += currentSpeed * orientation;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    position -= currentSpeed * orientation;

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    position -= currentSpeed * right;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    position += currentSpeed * right;

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    position += currentSpeed * actualUp;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    position -= currentSpeed * actualUp;

  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    this->baseSpeed = 0.1f;
  }
}

void Camera::processMouse(GLFWwindow* window)
{
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);   // Desabilitar cursor
    
    if (firstClick) {
      glfwSetCursorPos(window, (float)width / 2, (float)height / 2); // Colocar no meio
      firstClick = false;
    }

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

    orientation = glm::normalize(newOrientation);

    glfwSetCursorPos(window, (float)width / 2, (float)height / 2);
  } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // volta o cursor ao normal
    firstClick = true;
  }
}

glm::mat4 Camera::getProjectionMatrix() const { return projection; }
glm::vec3 Camera::getPosition() const { return position; }
