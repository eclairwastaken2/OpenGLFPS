#include "cameraController.h"

CameraController::CameraController(Camera& camera, float startX, float startY)
	:camera_(camera), lastX(startX), lastY(startY) { }

void CameraController::processKeyboard(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_.ProcessKeyboard(FORWARD, deltaTime); 
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_.ProcessKeyboard(BACKWARD, deltaTime); 
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_.ProcessKeyboard(RIGHT, deltaTime);
}


void CameraController::onMouseMove(double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera_.ProcessMouseMovement(xoffset, yoffset);
}

void CameraController::onScroll(double yoffset)
{
    camera_.ProcessMouseScroll(static_cast<float>(yoffset));
}