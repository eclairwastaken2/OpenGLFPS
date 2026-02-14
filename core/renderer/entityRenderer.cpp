#include <glad/glad.h>

#include "animation/animator.h"
#include "animation/model_animation.h"
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
    staticShader_->use();
    staticShader_->setVec3("flashlight.ambient", 0.1f, 0.1f, 0.1f);
    staticShader_->setVec3("flashlight.diffuse", 0.8f, 0.8f, 0.8f);
    staticShader_->setVec3("flashlight.specular", 1.0f, 1.0f, 1.0f);

    staticShader_->setFloat("flashlight.constant", 1.0f);
    staticShader_->setFloat("flashlight.linear", 0.09f);
    staticShader_->setFloat("flashlight.quadratic", 0.032f);

    staticShader_->setFloat("material.shininess", 32.0f);
    staticShader_->setInt("material.diffuse", 0);
    staticShader_->setInt("material.specular", 1);

    staticShader_->setVec3("emissiveColor", 0.0f, 0.0f, 0.0f);
    staticShader_->setFloat("emissiveStrength", 0.0f);
    staticShader_->setBool("isEmissive", false);

    staticShader_->setInt("numPointLights", 0);

}

void Renderer::begin(const Camera& camera)
{
	modelBatches_.clear(); 
	meshBatches_.clear(); 
	//animatedDraws_.clear(); 

	glm::mat4 projection = glm::perspective(
		glm::radians(camera.Zoom),
		(float)1920 / (float)1080,
		0.1f,
		100.0f
	);
	view_ = camera.GetViewMatrix();
	projection_ = projection; 

    staticShader_->use();
    staticShader_->setVec3("flashlight.position", camera.Position);
    staticShader_->setVec3("flashlight.direction", camera.Front);
    staticShader_->setFloat("flashlight.cutOff", glm::cos(glm::radians(20.5f)));
    staticShader_->setFloat("flashlight.outerCutOff", glm::cos(glm::radians(25.5f)));
    staticShader_->setVec3("viewPos", camera.Position);
}

void Renderer::submitMesh(const Mesh& mesh, 
	const Material& material, 
	const glm::mat4& transform)
{
	meshBatches_.push_back({ &mesh, &material, transform }); 
}

void Renderer::submitModel(const Model& model,
	const glm::mat4& transform)
{
	modelBatches_.push_back({ &model, transform });
}

void Renderer::submitAnimated(const Model& model,
	const Animator& animator,
	const glm::mat4& transform)
{
	animatedDraws_.push_back({ &model, &animator, transform }); 
}


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
    const Material& material,
    const std::vector<glm::mat4>& transforms)
{
    staticShader_->use();
    staticShader_->setMat4("view", view_);
    staticShader_->setMat4("projection", projection_);
    material.bind();
    mesh.bind();
    std::cout << mesh.instanceVBO << std::endl;

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

void Renderer::renderAnimatedPass()
{
    animatedShader_->use(); 
    animatedShader_->setMat4("projection", projection_);
    animatedShader_->setMat4("view", view_);

    for (auto& draw : animatedDraws_)
    {
        animatedShader_->setMat4("model", draw.transform); 

        auto boneMatrices = draw.animator->GetFinalBoneMatrices();
        for (int i = 0; i < boneMatrices.size(); ++i)
        {
            animatedShader_->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", boneMatrices[i]);
        }

        draw.model->Draw(*animatedShader_);

    }
}

void Renderer::renderMeshPass()
{
    staticShader_->use();
    staticShader_->setMat4("view", view_);
    staticShader_->setMat4("projection", projection_);

    struct Key
    {
        const Mesh* mesh;
        const Material* material;

        bool operator==(const Key& other) const
        {
            return mesh == other.mesh && material == other.material;
        }
    };

    struct KeyHasher
    {
        std::size_t operator()(const Key& k) const
        {
            return std::hash<const void*>()(k.mesh) ^
                std::hash<const void*>()(k.material);
        }
    };

    std::unordered_map<Key, std::vector<glm::mat4>, KeyHasher> batches;

    for (auto& draw : meshBatches_)
    {
        batches[{ draw.mesh, draw.material }].push_back(draw.transform);
    }

    for (auto& [key, transforms] : batches)
    {
        drawInstancedBatch(*key.mesh, *key.material, transforms);
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

