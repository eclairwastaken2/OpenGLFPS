#include "window.h"
#include <stdexcept>

Window::Window(int w, int h, const char* title)
{
	if (!glfwInit())
		throw std::runtime_error("GLFW init failed");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_ = glfwCreateWindow(w, h, title, nullptr, nullptr);
	if (!window_)
		throw std::runtime_error("Failed to create window");

	glfwMakeContextCurrent(window_);
}

Window::~Window()
{
	glfwDestroyWindow(window_);
	glfwTerminate();
}


GLFWwindow* Window::get() const { return window_; }

