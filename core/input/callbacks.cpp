#include "callbacks.h"
#include "../camera/cameraController.h"

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	auto* controller =
		static_cast<CameraController*>(glfwGetWindowUserPointer(window));

	if (controller)
		controller->onMouseMove(xpos, ypos);
}

static void scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset)
{
	auto* controller =
		static_cast<CameraController*>(glfwGetWindowUserPointer(window));

	if (controller)
		controller->onScroll(yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	glViewport(0, 0, width, height);
}


void registerCallbacks(GLFWwindow* window) {
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}