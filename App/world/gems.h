#pragma once
#include "gameObject.h"
#include "renderer/entityRenderer.h"
#include <world/level.h>
#include <glm/glm.hpp>
#include <geometry/cube.h>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/texture2d.h>
#include <gl/material.h>
#include <gl/shader.h>
#include <gl/renderParams.h>
#include <gl/model.h>
#include <iostream>
#include <memory>
#include <gl/shaderManager.h>


class Gems : public GameObject
{
public:
	Gems()
		: glow_({ 1.0f, 0.8f, 0.1f }, 1.0f),
		floorShader_(ShaderManager::get("light")),
		model_("Assets/resources/objects/gems/source/gems.obj",
			floorShader_,
			glow_)
	{
	
	}

	void update(float dt) override
	{

	}

	void render(Renderer& renderer) override
	{
		int numPointLight = 0; 
		for (int z = 0; z < level_.getH(); z++)
		{
			for (int x = 0; x < level_.getW(); x++)
			{
				if (level_.at(x, z) == 'B')
				{
					glm::mat4 gemModel = glm::mat4(1.0f);
					gemModel = glm::translate(gemModel, glm::vec3((float)x, -0.5f, (float)z));
					glm::vec3 scaleVector = glm::vec3(0.2f, 0.2f, 0.2f);
					gemModel = glm::scale(gemModel, scaleVector);
					renderer.submitModel(model_, gemModel); 
					std::string base = "pointLights[" + std::to_string(numPointLight) + "]";
					floorShader_->setVec3(base + ".position", glm::vec3((float)x, 0.0f, (float)z));
					floorShader_->setVec3(base + ".ambient", 1.0f, 0.8f, 0.1f);
					floorShader_->setVec3(base + ".diffuse", 1.0f, 0.8f, 0.1f);
					floorShader_->setVec3(base + ".specular", 1.0f, 0.8f, 0.1f);

					floorShader_->setFloat(base + ".constant", 1.0f);
					floorShader_->setFloat(base + ".linear", 0.7f);
					floorShader_->setFloat(base + ".quadratic", 1.8f);
					numPointLight++;
				}
			}
		}
		floorShader_->setInt("numPointLights", numPointLight); 

	}

private:
	Level level_ = Level("Assets/levels/level.txt");
	RenderParams glow_;
	std::shared_ptr<Shader> floorShader_;
	Model model_;
};