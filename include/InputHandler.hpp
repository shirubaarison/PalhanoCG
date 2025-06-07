#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class InputHandler {
  GLFWwindow* window;
  double lastX;
  double lastY;

public:
   explicit InputHandler(GLFWwindow* win) : window(win) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        // centro
        lastX = static_cast<double>(width) / 2.0;
        lastY = static_cast<double>(height) / 2.0;
    }

    bool isKeyPressed(int key) const {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    bool isKeyReleased(int key) const {
        return glfwGetKey(window, key) == GLFW_RELEASE;
    }

    bool isMouseButtonPressed(int button) const {
        return glfwGetMouseButton(window, button) == GLFW_PRESS;
    }

    void getMousePosition(double& x, double& y) const {
        glfwGetCursorPos(window, &x, &y);
    }

    void setCursorPosition(double x, double y) {
        glfwSetCursorPos(window, x, y);
    }

    glm::vec2 getMouseOffset() {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        glm::vec2 offset(xpos - lastX, ypos - lastY);
        
        lastX = xpos;
        lastY = ypos;

        return offset;
    }

    void centerCursor() {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        lastX = static_cast<double>(width) / 2.0;
        lastY = static_cast<double>(height) / 2.0;
        glfwSetCursorPos(window, lastX, lastY);
    }

    void setCursorMode(bool disabled) {
        glfwSetInputMode(window, GLFW_CURSOR,
                         disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }};

#endif
