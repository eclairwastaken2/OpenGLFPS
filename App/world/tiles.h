#include "gameObject.h"
#include "renderer/entityRenderer.h"
#include <world/level.h>
#include <glm/glm.hpp>
#include <geometry/cube.h>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/texture2d.h>
#include <gl/material.h>
#include <iostream>

class Tiles : public GameObject
{
public:
	Tiles()
	{
		floorMaterial_.setTexture(woodTexture_);
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
					floorMaterial_,
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
					floorMaterial_,
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
						floorMaterial_,
						model
					);
				}
			}
		}
	}

private:
	Level level_ = Level("Assets/levels/level.txt");
	Mesh cubeMesh_ = MakeCube();
	std::shared_ptr<Texture2D> woodTexture_ = 
		std::make_shared<Texture2D>(
		Texture2D::Builder()
		.wrap(GL_REPEAT, GL_REPEAT)
		.filter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR)
		.fromFile("Assets/textures/container.jpg")
	);
	Material floorMaterial_;
};