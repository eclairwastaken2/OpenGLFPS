#pragma once
#include <glad/glad.h>


#include <GLFW/glfw3.h>

#include "camera/camera.h"
#include "camera/cameraController.h"
#include "world/level.h"
#include "world/player.h"
#include "worldDraw/levelRenderer.h"
#include "worldDraw/levelVisuals.h"

#include "application/layer.h"
#include "application/inputEvents.h"
#include "application/windowEvents.h"


class GameLayer : public Core::Layer
{
public:
    GameLayer();
    virtual ~GameLayer(); 

    virtual void OnEvent(Core::Event& event);
       
    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;

private:
    GLFWwindow* m_Window;

    Camera m_Camera;
    CameraController m_CameraController;
    Player m_Player;
    Level m_Level;

    Shader m_LightingShader;
    Shader m_AnimationShader;

    Model m_StaticModel;
    Model m_AnimationModel;

    Animation m_DanceAnimation;
    Animator m_Animator;

    LevelVisuals m_Visuals;
    LevelRenderer m_Renderer;
};