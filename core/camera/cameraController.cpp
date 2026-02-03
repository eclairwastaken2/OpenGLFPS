#include "cameraController.h"

CameraController::CameraController(Camera& camera, float startX, float startY)
	:camera_(camera), lastX_(startX), lastY_(startY) { }

void CameraController::onUpdate(float dt)
{
    if (moveForward_)
        camera_.ProcessKeyboard(FORWARD, dt);
    if (moveBackward_)
        camera_.ProcessKeyboard(BACKWARD, dt);
    if (moveLeft_)
        camera_.ProcessKeyboard(LEFT, dt);
    if (moveRight_)
        camera_.ProcessKeyboard(RIGHT, dt);
}

void CameraController::onEvent(Core::Event& e)
{
    Core::EventDispatcher dispatcher(e);

    dispatcher.Dispatch<Core::MouseMovedEvent>(
        [this](auto& e) { return onMouseMoved(e); });

    dispatcher.Dispatch<Core::MouseScrolledEvent>(
        [this](auto& e) { return onMouseScrolled(e); });

    dispatcher.Dispatch<Core::KeyPressedEvent>(
        [this](auto& e) { return onKeyPressed(e); });

    dispatcher.Dispatch<Core::KeyReleasedEvent>(
        [this](auto& e) { return onKeyReleased(e); });
}

bool CameraController::onMouseMoved(Core::MouseMovedEvent& e)
{
    float xpos = (float)e.GetX();
    float ypos = (float)e.GetY();

    if (firstMouse_)
    {
        lastX_ = xpos;
        lastY_ = ypos;
        firstMouse_ = false;
        return false;
    }

    float xoffset = xpos - lastX_;
    float yoffset = lastY_ - ypos;

    lastX_ = xpos;
    lastY_ = ypos;

    camera_.ProcessMouseMovement(xoffset, yoffset);
    return false;
}

bool CameraController::onMouseScrolled(Core::MouseScrolledEvent& e)
{
    camera_.ProcessMouseScroll((float)e.GetYOffset());
    return false;
}

bool CameraController::onKeyPressed(Core::KeyPressedEvent& e)
{
    if (e.IsRepeat())
        return false;

    switch (e.GetKeyCode())
    {
    case GLFW_KEY_W: moveForward_ = true; break;
    case GLFW_KEY_S: moveBackward_ = true; break;
    case GLFW_KEY_A: moveLeft_ = true; break;
    case GLFW_KEY_D: moveRight_ = true; break;
    }
    return false;
}

bool CameraController::onKeyReleased(Core::KeyReleasedEvent& e)
{
    switch (e.GetKeyCode())
    {
    case GLFW_KEY_W: moveForward_ = false; break;
    case GLFW_KEY_S: moveBackward_ = false; break;
    case GLFW_KEY_A: moveLeft_ = false; break;
    case GLFW_KEY_D: moveRight_ = false; break;
    }
    return false;
}