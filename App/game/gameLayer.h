#pragma once

#include "application/layer.h"

#include "world/player.h"
#include "world/level.h"
#include <gl/mesh.h>
#include <gl/texture2d.h>
#include <gl/shader.h>
#include <geometry/cube.h>
#include <glad/glad.h>
#include "camera/camera.h"
#include "camera/cameraController.h"


class GameLayer : public Core::Layer
{
public:
    GameLayer();
    virtual ~GameLayer(); 

  /*  virtual void onEvent(Core::Event& event) override;
    virtual void onUpdate(float ts) override; */
    virtual void onRender() override; 

private:
    void setupLighting();
    void updateLighting(Shader& shader, const Camera& camera);

private:
    //// World
    Level level_;
    Mesh cubeMesh_;

    Texture2D woodTexture_;

    Shader lightingShader_;
 
    //// Camera
    Camera camera_;
    CameraController cameraController_;

    //// Rendering
    //std::unique_ptr<Shader> lightingShader_;
    //std::unique_ptr<Shader> animationShader_;

    //LevelVisuals visuals_;
    //LevelRenderer renderer_;

    //// Animation
  /*  Animation danceAnimation_;
    Animator animator_;*/

    //// Timing
    //float lastFrame_ = 0.0f;
};