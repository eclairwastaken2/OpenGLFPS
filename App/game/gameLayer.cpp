#include "gameLayer.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <gl/mesh.h>
#include <gl/texture2d.h>
#include <gl/shader.h>
#include <geometry/cube.h>
#include <glad/glad.h>
#include <world/player.h>


static constexpr float CAMERA_RADIUS = 0.3f;

GameLayer::GameLayer()
    : level_("Assets/levels/level.txt")
    , cubeMesh_(MakeCube())
    , woodTexture_(std::make_shared<Texture2D>(
        Texture2D::Builder()
        .wrap(GL_REPEAT, GL_REPEAT)
        .filter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR)
        .fromFile("Assets/textures/container.jpg")
    ))
    ,camera_(level_.findSpawn())
    , cameraController_(camera_, 1920 / 2.0f, 1080 / 2.0f)

{
    player_.setPosition(level_.findSpawn());
    glEnable(GL_DEPTH_TEST);
    //player_.setPosition(level_.findSpawn());\
    //floorMaterial_.setTexture("material.diffuse", &woodTexture_, 0);
    floorMaterial_.setTexture( woodTexture_);
    setupLighting();

}

GameLayer::~GameLayer()
{

}



void GameLayer::onRender()
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer_.begin(camera_); 


    for (int z = 0; z < level_.getH(); z++)
    {
        for (int x = 0; x < level_.getW(); x++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(
                model,
                glm::vec3(x + 0.5f, 0.0f, z + 0.5f)
            );


            renderer_.submitMesh(
                cubeMesh_,
                floorMaterial_,
                model
            );
        }
    }
    renderer_.end();
}

void GameLayer::onUpdate(float dt)
{
    cameraController_.onUpdate(dt);
}

void GameLayer::onEvent(Core::Event& e)
{
    std::cout << e.ToString() << std::endl;
    cameraController_.onEvent(e);
}


void GameLayer::setupLighting()
{
    //lightingShader_.setVec3("flashlight.ambient", 0.1f, 0.1f, 0.1f);
    //lightingShader_.setVec3("flashlight.diffuse", 0.8f, 0.8f, 0.8f);
    //lightingShader_.setVec3("flashlight.specular", 1.0f, 1.0f, 1.0f);

    //lightingShader_.setFloat("flashlight.constant", 1.0f);
    //lightingShader_.setFloat("flashlight.linear", 0.09f);
    //lightingShader_.setFloat("flashlight.quadratic", 0.032f);

    //lightingShader_.setFloat("material.shininess", 32.0f);
    //lightingShader_.setInt("material.diffuse", 0);
    //lightingShader_.setInt("material.specular", 1);
}


void GameLayer::updateLighting()
{

    //lightingShader_.setVec3("flashlight.position", camera_.Position);
    //lightingShader_.setVec3("flashlight.direction", camera_.Front);
    //lightingShader_.setFloat("flashlight.cutOff", glm::cos(glm::radians(20.5f)));
    //lightingShader_.setFloat("flashlight.outerCutOff", glm::cos(glm::radians(25.5f)));

    //lightingShader_.setVec3("viewPos", camera_.Position);

    //glm::mat4 projection = glm::perspective(
    //    glm::radians(camera_.Zoom),
    //    (float)1920 / (float)1080,
    //    0.1f,
    //    100.0f
    //);

    //lightingShader_.setMat4("projection", projection);
    //lightingShader_.setMat4("view", camera_.GetViewMatrix());
}