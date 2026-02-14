#include "gameLayer.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <gl/shader.h>
#include <glad/glad.h>
#include <world/player.h>

static constexpr float CAMERA_RADIUS = 0.3f;

GameLayer::GameLayer()
    : level_("Assets/levels/level.txt")
    ,camera_(level_.findSpawn())
    , cameraController_(camera_, 1920 / 2.0f, 1080 / 2.0f)

{
    player_.setPosition(level_.findSpawn());
    glEnable(GL_DEPTH_TEST);

}

GameLayer::~GameLayer()
{

}



void GameLayer::onRender()
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer_.begin(camera_); 
    tiles_.render(renderer_); 
    renderer_.end();
}

void GameLayer::onUpdate(float dt)
{
    glm::vec3 movement = cameraController_.getMovement(dt);

    glm::vec3 pos = camera_.Position;

    glm::vec3 tryX = pos;
    tryX.x += movement.x;

    if (!level_.collides(tryX.x, tryX.z, CAMERA_RADIUS))pos.x = tryX.x;

    glm::vec3 tryZ = pos;
    tryZ.z += movement.z;

    if (!level_.collides(tryZ.x, tryZ.z, CAMERA_RADIUS))pos.z = tryZ.z;

    camera_.Position = pos;
}

void GameLayer::onEvent(Core::Event& e)
{
    std::cout << e.ToString() << std::endl;
    cameraController_.onEvent(e);
}