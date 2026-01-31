#pragma once

#include "application/layer.h"

#include "world/player.h"
#include "world/level.h"

#include "gl/shader.h"

#include "camera/camera.h"
#include "camera/cameraController.h"

#include "animation/animator.h"
#include "animation/model_animation.h"

#include "worldDraw/levelRenderer.h"
#include "worldDraw/levelVisuals.h"


class GameLayer : public Core::Layer
{
public:
    GameLayer();
    virtual ~GameLayer(); 

    //virtual void onEvent(Core::Event& event) override;
    //virtual void onUpdate(float ts) override; 
    virtual void onRender() override; 

private:
    void setupLighting();
    void updateLighting();

private:
    // World
    Level level_;
    Player player_;

    // Camera
    Camera camera_;
    CameraController cameraController_;

    // Rendering
    Shader lightingShader_;
    Shader animationShader_;

    LevelVisuals visuals_;
    LevelRenderer renderer_;

    // Animation
    Animation danceAnimation_;
    Animator animator_;

    // Timing
    float lastFrame_ = 0.0f;
};