#pragma once
#include <GLFW/glfw3.h>
#include "camera.h"

class CameraController
{
public:
	CameraController(Camera& camera, float startX, float startY); 

	void processKeyboard(GLFWwindow* window, float deltaTime); 
	void onMouseMove(double xpos, double ypos); 
	void onScroll(double yoffset); 

private:
	Camera& camera_; 

	bool firstMouse = true; 
	float lastX; 
	float lastY; 
};

