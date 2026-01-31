#include "gameLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

#include <glad/glad.h>

static constexpr float CAMERA_RADIUS = 0.3f;

GameLayer::GameLayer()
    /*: level_("Assets/levels/level.txt"),
    camera_(level_.findSpawn()),
    cameraController_(camera_, 900.0f, 800.0f),
    danceAnimation_(
        "Assets/resources/objects/vampire/dancing_vampire.dae",
        nullptr),
    animator_(&danceAnimation_),
    renderer_(visuals_)*/
{
    //player_.setPosition(level_.findSpawn());
    //setupLighting();
}

GameLayer::~GameLayer()
{
 
}

void GameLayer::onAttach()
{
    //lightingShader_ = std::make_unique<Shader>(
    //    "Assets/shaders/light_caster.vs",
    //    "Assets/shaders/light_caster.fs"
    //);

    //animationShader_ = std::make_unique<Shader>(
    //    "Assets/shaders/anim_model.vs",
    //    "Assets/shaders/anim_model.fs"
    //);

    //setupLighting();
}

void GameLayer::onRender()
{
    std::cout << "hmmm" << std::endl; 
  /*  animator_.UpdateAnimation(deltaTime);

    glm::vec3 oldPos = camera_.Position;

    cameraController_.processKeyboard(nullptr, deltaTime);

    glm::vec3 nextPos = camera_.Position;

    if (level_.collides(nextPos.x, oldPos.z, CAMERA_RADIUS))
        nextPos.x = oldPos.x;

    if (level_.collides(nextPos.x, nextPos.z, CAMERA_RADIUS))
        nextPos.z = oldPos.z;

    camera_.Position = nextPos;
    player_.setPosition(nextPos);
    player_.tryCollect(level_);

    updateLighting();

    renderer_.render(level_);*/
}


void GameLayer::setupLighting()
{
    //lightingShader_->use();

    //lightingShader_->setVec3("flashlight.ambient", 0.1f, 0.1f, 0.1f);
    //lightingShader_->setVec3("flashlight.diffuse", 0.8f, 0.8f, 0.8f);
    //lightingShader_->setVec3("flashlight.specular", 1.0f, 1.0f, 1.0f);

    //lightingShader_->setFloat("flashlight.constant", 1.0f);
    //lightingShader_->setFloat("flashlight.linear", 0.09f);
    //lightingShader_->setFloat("flashlight.quadratic", 0.032f);

    //lightingShader_->setFloat("material.shininess", 32.0f);
}

void GameLayer::updateLighting()
{
   /* lightingShader_->use();

    lightingShader_->setVec3("flashlight.position", camera_.Position);
    lightingShader_->setVec3("flashlight.direction", camera_.Front);
    lightingShader_->setVec3("viewPos", camera_.Position);

    glm::mat4 projection = glm::perspective(
        glm::radians(camera_.Zoom),
        1800.0f / 1600.0f,
        0.1f,
        100.0f
    );

    lightingShader_->setMat4("projection", projection);
    lightingShader_->setMat4("view", camera_.GetViewMatrix());*/
}