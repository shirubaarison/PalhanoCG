#include "engine/core/Window.hpp"
#include <iostream>

Window::Window() {}

bool Window::initialize(int width, int height, const char* title)
{
	this->width = width;
  this->height = height;

  if (!glfwInit()) {
		std::cerr << "Falha ao inicializar GLFW." << std::endl;
		return false;
	}

  // OpenGL versão 4.3 Core profile (porque tem info de debug)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	mWindow = glfwCreateWindow(width, height, title, NULL, NULL);
  if (mWindow == NULL) {
    std::cerr << "Falha ao criar janela GLFW." << std::endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(mWindow);

	const auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(mWindow, (mode->width / 2) - width / 2, (mode->height / 2) - height / 2);

	glfwSetWindowUserPointer(mWindow, this);
	glfwSetWindowSizeCallback(mWindow, Window::window_size_callback);

	glfwFocusWindow(mWindow);
	
	std::cout << "[GLFW] Janela inicializada com sucesso" << std::endl;

	return true;
}

void Window::shutdown() {
	glfwDestroyWindow(mWindow);
	glfwTerminate();
	mWindow = nullptr;
}

bool Window::windowShouldClose()
{
	return glfwWindowShouldClose(mWindow);
}

GLFWwindow* Window::getWindow() const
{
	return mWindow;
}

void Window::swapBuffers()
{
	glfwSwapBuffers(mWindow);
}

int Window::getWidth()
{
	return width;
}

int Window::getHeight()
{
	return height;
}

void Window::window_size_callback(GLFWwindow* window, int width, int height)
{
	// TODO: arrumar o aspect ratio
	Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (self) {
		self->handle_window_resize(width, height);
	}
}

void Window::handle_window_resize(int width, int height)
{
	glViewport(0, 0, width, height);
}
