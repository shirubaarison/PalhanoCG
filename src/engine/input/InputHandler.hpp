#ifndef INPUT_HANDLER_CLASS_H
#define INPUT_HANDLER_CLASS_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class InputHandler {
public: 
  InputHandler();
  bool initialize(GLFWwindow* win);

  bool isReady();
  bool isKeyPressed(int key);
  bool isKeyReleased(int key);
  bool isMouseButtonPressed(int button);

  void getMousePosition(double& x, double& y);
  void setCursorPosition(double x, double y);

  glm::vec2 getMouseOffset();

  void centerCursor();
  void setCursorMode(bool disabled);
private:
  GLFWwindow* window = nullptr;
  double lastX = 0.0;
  double lastY = 0.0;
  bool initialized = false;
};

#endif
