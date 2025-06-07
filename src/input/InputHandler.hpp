#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace InputHandler {

void initialize(GLFWwindow* win);

bool isKeyPressed(int key);
bool isKeyReleased(int key);
bool isMouseButtonPressed(int button);

void getMousePosition(double& x, double& y);
void setCursorPosition(double x, double y);

glm::vec2 getMouseOffset();

void centerCursor();
void setCursorMode(bool disabled);

} 

#endif
