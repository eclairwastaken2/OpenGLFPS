#pragma once
#include <GLFW/glfw3.h>
#include "camera.h"
#include <application/inputEvents.h>

class CameraController
{
public:
	CameraController(Camera& camera, float startX, float startY); 
 
	void onEvent(Core::Event& e); 
	glm::vec3 getMovement(float dt) const;


private:
	// event handlers
	bool onMouseMoved(Core::MouseMovedEvent& e);
	bool onMouseScrolled(Core::MouseScrolledEvent& e);
	bool onKeyPressed(Core::KeyPressedEvent& e);
	bool onKeyReleased(Core::KeyReleasedEvent& e);

private:
	Camera& camera_; 

	// movement state
	bool moveForward_ = false;
	bool moveBackward_ = false;
	bool moveLeft_ = false;
	bool moveRight_ = false;

	// mouse state
	bool firstMouse_ = true;
	float lastX_;
	float lastY_;
};

