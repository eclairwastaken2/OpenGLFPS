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
#include <gl/model.h>
#include <iostream>
#include <memory>
#include <gl/shaderManager.h>


class Gems : public GameObject
{
public:
	Gems()
		: floorShader_(ShaderManager::get("light")),
		model_("Assets/resources/objects/gems/source/gems.obj", floorShader_)
	{
	}

	void update(float dt) override
	{

	}

	void render(Renderer& renderer) override
	{
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
				}
			}
		}
	}

private:
	Level level_ = Level("Assets/levels/level.txt");
	std::shared_ptr<Shader> floorShader_;
	Model model_;
};