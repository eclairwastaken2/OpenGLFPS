#pragma once

#include "application/layer.h"

#include "world/player.h"
#include "world/level.h"
#include <gl/mesh.h>
#include <gl/texture2d.h>
#include <gl/shader.h>
#include <gl/material.h>
#include <geometry/cube.h>
#include <glad/glad.h>
#include "camera/camera.h"
#include "camera/cameraController.h"
#include "renderer/entityRenderer.h"

class GameLayer : public Core::Layer
{
public:
    GameLayer();
    virtual ~GameLayer(); 

    virtual void onEvent(Core::Event& event) override;
    virtual void onUpdate(float ts) override; 
    virtual void onRender() override; 

private:
    void setupLighting();
    void updateLighting();

private:

    Player player_; 
    //// World
    Level level_;
    Mesh cubeMesh_;
    Material floorMaterial_; 

    std::shared_ptr<Texture2D> woodTexture_;
 
    //// Camera
    Camera camera_;
    CameraController cameraController_;
    
    Renderer renderer_; 

    bool OnMouseMoved(Core::MouseMovedEvent& event);
};