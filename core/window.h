#pragma once
#include <GLFW/glfw3.h>

class Window {
public:
	Window(int w, int h, const char* title);
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	GLFWwindow* get() const;

private:
	GLFWwindow* window_;
};