#include <glad/glad.h>

//#include "animation/animator.h"
#include "gl/model.h"
#include "camera/camera.h"
#include "gl/mesh.h"
#include "gl/shader.h"
#include "gl/material.h"
#include <glm/gtc/matrix_transform.hpp>
#include "gl/texture2d.h"
#include "entityRenderer.h"

Renderer::Renderer()
{
    staticShader_ = new Shader("Assets/shaders/light_caster.vs", "Assets/shaders/light_caster.fs"); 
    animatedShader_ = new Shader("Assets/shaders/anim_model.vs", "Assets/shaders/anim_model.fs"); 
}

void Renderer::applyFrameUniforms(Shader& shader)
{
    shader.setVec3("flashlight.ambient", 0.1f, 0.1f, 0.1f);
    shader.setVec3("flashlight.diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("flashlight.specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("flashlight.constant", 1.0f);
    shader.setFloat("flashlight.linear", 0.09f);
    shader.setFloat("flashlight.quadratic", 0.032f);
    shader.setFloat("material.shininess", 32.0f);
    //shader.setInt("material.diffuse", 0);
    //shader.setInt("material.specular", 1);
    //shader.setVec3("emissiveColor", 0.0f, 0.0f, 0.0f);
    //shader.setFloat("emissiveStrength", 0.0f);
    //shader.setBool("isEmissive", false);

    shader.setInt("numPointLights", 0);

    shader.setVec3("flashlight.position", camera_.Position);
    shader.setVec3("flashlight.direction", camera_.Front);
    shader.setFloat("flashlight.cutOff", glm::cos(glm::radians(20.5f)));
    shader.setFloat("flashlight.outerCutOff", glm::cos(glm::radians(25.5f)));
    shader.setVec3("viewPos", camera_.Position);
}

void Renderer::begin(const Camera& camera)
{
	modelBatches_.clear(); 
	meshBatches_.clear(); 
	//animatedDraws_.clear(); 
    camera_ = camera; 
	glm::mat4 projection = glm::perspective(
		glm::radians(camera.Zoom),
		(float)1920 / (float)1080,
		0.1f,
		100.0f
	);
	view_ = camera.GetViewMatrix();
	projection_ = projection; 
}

void Renderer::submitMesh(std::shared_ptr<Mesh> mesh,
    const glm::mat4& transform)
{
    meshBatches_.push_back({ mesh, transform });
}

void Renderer::submitModel(const Model& model,
	const glm::mat4& transform)
{
    for (const auto& mesh : model.meshes)
    {
        submitMesh(mesh, transform); 
    }
}

//void Renderer::submitAnimated(const Model& model,
//	const Animator& animator,
//	const glm::mat4& transform)
//{
//	animatedDraws_.push_back({ &model, &animator, transform }); 
//}


//void Renderer::drawInstancedBatch(const Model& model,
//    const std::vector<glm::mat4>& transforms)
//{
//    GLuint instanceVBO;
//    glGenBuffers(1, &instanceVBO);
//    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
//    glBufferData(GL_ARRAY_BUFFER,
//        transforms.size() * sizeof(glm::mat4),
//        transforms.data(),
//        GL_STATIC_DRAW);
//
//    for (int i = 0; i < 4; i++)
//    {
//        glEnableVertexAttribArray(4 + i);
//        glVertexAttribPointer(4 + i,
//            4,
//            GL_FLOAT,
//            GL_FALSE,
//            sizeof(glm::mat4),
//            (void*)(sizeof(glm::vec4) * i));
//        glVertexAttribDivisor(4 + i, 1);
//    }
//
//    glDrawElementsInstanced(
//        GL_TRIANGLES,
//        model.indexCount(), 
//        GL_UNSIGNED_INT,
//        0,
//        transforms.size()
//    );
//
//    glDeleteBuffers(1, &instanceVBO);
//}

void Renderer::drawInstancedBatch(const Mesh& mesh,
    const std::vector<glm::mat4>& transforms)
{
    mesh.bind();
    std::cout << mesh.instanceVBO << std::endl;

    auto& material = *mesh.material_;
    material.bind();

    Shader& shader = const_cast<Shader&>(material.getShader());
    shader.setMat4("view", view_);
    shader.setMat4("projection", projection_);
    applyFrameUniforms(shader); 
    mesh.bind(); 
        
    glBindBuffer(GL_ARRAY_BUFFER, mesh.instanceVBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        transforms.size() * sizeof(glm::mat4),
        transforms.data(),
        GL_DYNAMIC_DRAW
    );

    glDrawElementsInstanced(
        GL_TRIANGLES,
        mesh.indexCount(),
        GL_UNSIGNED_INT,
        0,
        transforms.size()
    );
}

//void Renderer::renderAnimatedPass()
//{
//    animatedShader_->use(); 
//    animatedShader_->setMat4("projection", projection_);
//    animatedShader_->setMat4("view", view_);
//
//    for (auto& draw : animatedDraws_)
//    {
//        animatedShader_->setMat4("model", draw.transform); 
//
//        auto boneMatrices = draw.animator->GetFinalBoneMatrices();
//        for (int i = 0; i < boneMatrices.size(); ++i)
//        {
//            animatedShader_->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", boneMatrices[i]);
//        }
//
//        draw.model->Draw(*animatedShader_);
//
//    }
//}

void Renderer::renderMeshPass()
{
    std::unordered_map<const Mesh*, std::vector<glm::mat4>> batches;

    for (auto& draw : meshBatches_)
    {
        batches[draw.mesh.get()].push_back(draw.transform);
    }

    for (auto& [mesh, transforms] : batches)
    {
        drawInstancedBatch(*mesh, transforms);
    }
}

void Renderer::end()
{
    //renderShadowPass();
    renderMeshPass();
    cout << "uhh" << endl; 
    //renderAnimatedPass();
}

//void Renderer::renderShadowPass()
//{
//    shadowShader_->use();
//
//    // set light view projection matrix here
//
//    for (auto& draw : staticDraws_)
//    {
//        shadowShader_->setMat4("u_Model", draw.transform);
//        draw.mesh->draw();
//    }
//
//    for (auto& draw : skinnedDraws_)
//    {
//        shadowShader_->setMat4("u_Model", draw.transform);
//        draw.model->draw(*shadowShader_);
//    }
//}

