#include "gameLayer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>

GameLayer::GameLayer(int width, int height, GLFWwindow* window)
    : m_Window(window),
    m_Level("../Assets/levels/level.txt"),
    m_Camera(m_Level.findSpawn()),
    m_CameraController(m_Camera, width / 2.0f, height / 2.0f),
    m_LightingShader("../Assets/shaders/light_caster.vs",
        "../Assets/shaders/light_caster.fs"),
    m_AnimationShader("../Assets/shaders/anim_model.vs",
        "../Assets/shaders/anim_model.fs"),
    m_StaticModel("../Assets/resources/objects/gems/source/gems.obj"),
    m_AnimationModel("../Assets/resources/objects/vampire/dancing_vampire.dae"),
    m_DanceAnimation("../Assets/resources/objects/vampire/dancing_vampire.dae",
        &m_AnimationModel),
    m_Animator(&m_DanceAnimation),
    m_Renderer(m_Visuals)
{
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    m_Player.setPosition(m_Level.findSpawn());

    m_Visuals.propModel = &m_StaticModel;
    m_Visuals.animationModel = &m_AnimationModel;
    m_Visuals.animation = &m_DanceAnimation;
    m_Visuals.animator = &m_Animator;
    m_Visuals.lightingShader = &m_LightingShader;
    m_Visuals.animationShader = &m_AnimationShader;
}

void GameLayer::OnUpdate(float dt)
{
    m_Animator.UpdateAnimation(dt);

    glm::vec3 oldPos = m_Camera.Position;

    m_CameraController.processKeyboard(m_Window, dt);

    glm::vec3 nextPos = m_Camera.Position;
    constexpr float CAMERA_RADIUS = 0.3f;

    if (m_Level.collides(nextPos.x, oldPos.z, CAMERA_RADIUS))
        nextPos.x = oldPos.x;

    if (m_Level.collides(nextPos.x, nextPos.z, CAMERA_RADIUS))
        nextPos.z = oldPos.z;

    m_Camera.Position = nextPos;
    m_Player.setPosition(nextPos);
    m_Player.tryCollect(m_Level);
}

void GameLayer::OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Renderer.render(m_Level);
}

void GameLayer::OnImGuiRender()
{
    ImGui::Begin("Debug");
    ImGui::Text("Camera Pos: %.2f %.2f %.2f",
        m_Camera.Position.x,
        m_Camera.Position.y,
        m_Camera.Position.z);
    ImGui::End();
}