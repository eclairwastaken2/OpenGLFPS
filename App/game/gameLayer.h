#pragma once

#include "application/layer.h"

#include "world/player.h"
#include "world/level.h"
#include <glad/glad.h>
#include "camera/camera.h"
#include "camera/cameraController.h"
#include "renderer/entityRenderer.h"
#include <world/tiles.h>
#include <world/gems.h>

class GameLayer : public Core::Layer
{
public:
    GameLayer();
    virtual ~GameLayer(); 

    virtual void onEvent(Core::Event& event) override;
    virtual void onUpdate(float ts) override; 
    virtual void onRender() override; 

private:

    Player player_; 
    // World
    Level level_;
    Tiles tiles_; 
    Gems gems_; 
 
    // Camera
    Camera camera_;
    CameraController cameraController_;
    
    Renderer renderer_; 

    bool OnMouseMoved(Core::MouseMovedEvent& event);
};