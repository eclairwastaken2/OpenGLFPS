#include "gameLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <gl/mesh.h>
#include <gl/texture2d.h>
#include <gl/shader.h>
#include <geometry/cube.h>
#include <glad/glad.h>

static constexpr float CAMERA_RADIUS = 0.3f;

GameLayer::GameLayer()
    : level_("Assets/levels/level.txt")
    , cubeMesh_(MakeCube())
    , woodTexture_(
        Texture2D::Builder()
        .wrap(GL_REPEAT, GL_REPEAT)
        .filter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR)
        .fromFile("Assets/textures/container.jpg")
    )
    , lightingShader_(
        "Assets/shaders/light_caster.vs",
        "Assets/shaders/light_caster.fs"
    )
    ,camera_(level_.findSpawn())
    , cameraController_(camera_, 1920 / 2.0f, 1080 / 2.0f)
{
    glEnable(GL_DEPTH_TEST);


    //player_.setPosition(level_.findSpawn());
    //setupLighting();
    lightingShader_.use();

    lightingShader_.setVec3("flashlight.ambient", 0.1f, 0.1f, 0.1f);
    lightingShader_.setVec3("flashlight.diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader_.setVec3("flashlight.specular", 1.0f, 1.0f, 1.0f);

    lightingShader_.setFloat("flashlight.constant", 1.0f);
    lightingShader_.setFloat("flashlight.linear", 0.09f);
    lightingShader_.setFloat("flashlight.quadratic", 0.032f);

    lightingShader_.setFloat("material.shininess", 32.0f);
    lightingShader_.setMat4("view", glm::mat4(1.0f));
    lightingShader_.setMat4(
        "projection",
        glm::ortho(-10.f, 10.f, -10.f, 10.f, -10.f, 10.f)
    );

}

GameLayer::~GameLayer()
{

}



void GameLayer::onRender()
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateLighting(lightingShader_, camera_);
    //lightingShader_.setMat4("model", glm::mat4(1.0f));
    //cubeMesh_.draw(lightingShader_);

    //lightingShader_.use();
    for (int z = 0; z < level_.getH(); z++)
    {
        for (int x = 0; x < level_.getW(); x++)
        {
            //FLOOR
            lightingShader_.setFloat("emissiveStrength", 0.0f);
            glm::mat4 floorModel = glm::mat4(1.0f);
            floorModel = glm::translate(
                floorModel,
                glm::vec3(x + 0.5f, 0.0f, z + 0.5f)
            );
            lightingShader_.setMat4("model", floorModel);
            cubeMesh_.draw(lightingShader_);
        }
    }
    lightingShader_.setMat4(
        "model",
        glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3))
    );
    cubeMesh_.draw(lightingShader_);

}


void GameLayer::setupLighting()
{

}

void GameLayer::updateLighting(Shader& shader, const Camera& camera)
{
    lightingShader_.use();

    lightingShader_.setVec3("flashlight.position", camera_.Position);
    lightingShader_.setVec3("flashlight.direction", camera_.Front);
    lightingShader_.setFloat("flashlight.cutOff", glm::cos(glm::radians(20.5f)));
    lightingShader_.setFloat("flashlight.outerCutOff", glm::cos(glm::radians(25.5f)));

    lightingShader_.setVec3("viewPos", camera_.Position);

    glm::mat4 projection = glm::perspective(
        glm::radians(camera_.Zoom),
        (float)1920 / (float)1080,
        0.1f,
        100.0f
    );

    lightingShader_.setMat4("projection", projection);
    lightingShader_.setMat4("view", camera_.GetViewMatrix());
}