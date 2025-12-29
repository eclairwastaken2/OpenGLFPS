#include "levelRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

LevelRenderer::LevelRenderer(const LevelVisuals& v) : visuals(v) {}

void LevelRenderer::render(Level& level, Shader& shader)
{
	visuals.woodTexture->bind(TextureSlot::WOOD);
	for (int z = 0; z < level.getH(); z++)
	{
		for (int x = 0; x < level.getW(); x++)
		{
			//FLOOR
			glm::mat4 floorModel = glm::mat4(1.0f);
			floorModel = glm::translate(
				floorModel,
				glm::vec3(x + 0.5f, -6.0f, z + 0.5f)
			);
			shader.setMat4("model", floorModel);
			visuals.cubeMesh->draw(shader); 

			//CEILING
			glm::mat4 ceilingModel = glm::mat4(1.0f);
			ceilingModel = glm::translate(
				ceilingModel,
				glm::vec3(x + 0.5f, 6.0f, z + 0.5f)
			);

			shader.setMat4("model", ceilingModel);
			visuals.cubeMesh->draw(shader);
		}
	}


	for (int z = 0; z < level.getH(); z++)
	{
		for (int x = 0; x < level.getW(); x++)
		{
			if (level.at(x, z) == '#')
			{
				visuals.woodTexture->bind(0);
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3((float)x, 0.0f, (float)z));
				shader.setMat4("model", model);
				visuals.cubeMesh->draw(shader);
			}
			else if (level.at(x, z) == 'B')
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3((float)x, 0.0f, (float)z));
				glm::vec3 scaleVector = glm::vec3(0.3f, 0.3f, 0.3f);
				model = glm::scale(model, scaleVector);
				shader.setMat4("model", model);
				visuals.propModel->Draw(shader);
			}
		}
	}
}