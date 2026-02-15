#include "gameObject.h"
#include "renderer/entityRenderer.h"
#include <world/level.h>
#include <glm/glm.hpp>
#include <geometry/cube.h>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/texture2d.h>
#include <gl/material.h>
#include <gl/shader.h>
#include <iostream>
#include <memory>

class Tiles : public GameObject
{
public:
	Tiles()
		: floorShader_(std::make_shared<Shader>(
			"Assets/shaders/light_caster.vs",
			"Assets/shaders/light_caster.fs")),
		floorMaterial_(floorShader_)
	{
		Texture2D tex =
			Texture2D::Builder()
			.wrap(GL_REPEAT, GL_REPEAT)
			.filter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR)
			.fromFile("Assets/textures/container.jpg");

		woodTexture_ = std::make_shared<Texture2D>(std::move(tex));

		floorMaterial_.setDiffuse(woodTexture_);
		cubeMesh_->material_ =
			std::make_shared<Material>(floorShader_);

		cubeMesh_->material_->setDiffuse(woodTexture_);
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
				//FLOOR
				glm::mat4 floorModel = glm::mat4(1.0f);
				floorModel = glm::translate(
				floorModel,
					glm::vec3(x + 0.5f, -6.0f, z + 0.5f)
				);
				renderer.submitMesh(
					cubeMesh_,
					floorModel
				);

				//CEILING
				glm::mat4 ceilingModel = glm::mat4(1.0f);
				ceilingModel = glm::translate(
					ceilingModel,
					glm::vec3(x + 0.5f, 6.0f, z + 0.5f)
				);

				renderer.submitMesh(
					cubeMesh_,
					ceilingModel
				);	
			}
		}

		for (int z = 0; z < level_.getH(); z++)
		{
			for (int x = 0; x < level_.getW(); x++)
			{
				if (level_.at(x, z) == '#')
				{
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3((float)x, 0.0f, (float)z));
					renderer.submitMesh(
						cubeMesh_,
						model
					);
				}
			}
		}
	}

private:
	Level level_ = Level("Assets/levels/level.txt");
	std::shared_ptr<Mesh> cubeMesh_ = std::make_shared<Mesh>(MakeCube());
	std::shared_ptr<Texture2D> woodTexture_;
	std::shared_ptr<Shader> floorShader_;
	Material floorMaterial_;
};