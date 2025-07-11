#include "engine/input/InputHandler.hpp"
#include <iostream>

InputHandler::InputHandler() {}

bool InputHandler::isReady()
{
  return (window != nullptr) && initialized;
}

bool InputHandler::initialize(GLFWwindow* win)
{
  window = win;

  if (window) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    lastX = static_cast<double>(width) / 2.0;
    lastY = static_cast<double>(height) / 2.0;
    initialized = true;
    std::cout << "[INPUT] Input inicializado com sucesso" << std::endl;
  } else {
    initialized = false;
  }
  return initialized;
}

bool InputHandler::isKeyPressed(int key) {
  if (!isReady()) return false;
  return glfwGetKey(window, key) == GLFW_PRESS;
}

bool InputHandler::isKeyReleased(int key) {
  if (!isReady()) return false;
  return glfwGetKey(window, key) == GLFW_RELEASE;
}

bool InputHandler::isMouseButtonPressed(int button) {
  if (!isReady()) return false;
  return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

void InputHandler::getMousePosition(double& x, double& y) {
  if (!isReady()) {
    x = 0.0;
    y = 0.0;
    return;
  }
  glfwGetCursorPos(window, &x, &y);
}

void InputHandler::setCursorPosition(double x, double y) {
  if (!isReady()) return;
  glfwSetCursorPos(window, x, y);
}

glm::vec2 InputHandler::getMouseOffset() {
  if (!isReady()) return glm::vec2(0.0f, 0.0f);

  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);

  glm::vec2 offset(static_cast<float>(xpos - lastX), static_cast<float>(ypos - lastY));

  lastX = xpos;
  lastY = ypos;

  return offset;
}

void InputHandler::centerCursor() {
  if (!isReady()) return;

  int width, height;
  glfwGetWindowSize(window, &width, &height);
  lastX = static_cast<double>(width) / 2.0;
  lastY = static_cast<double>(height) / 2.0;
  glfwSetCursorPos(window, lastX, lastY);
}

void InputHandler::setCursorMode(bool disabled) {
  if (!isReady()) return;
  glfwSetInputMode(window, GLFW_CURSOR,
                   disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}
