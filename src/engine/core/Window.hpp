#ifndef WINDOW_CLASS_H
#define WINDOW_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
	Window();

	bool initialize();
	void shutdown();
	bool windowShouldClose();
	GLFWwindow* getWindow() const;
	void update();

	// para redimensionar a janela
  static void window_size_callback(GLFWwindow* window, int width, int height);
	void handle_window_resize(int width, int height);
private:
  GLFWwindow *mWindow;
};

#endif
