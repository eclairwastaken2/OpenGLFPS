//#include "levelRenderer.h"
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//LevelRenderer::LevelRenderer(const LevelVisuals& v) : visuals(v) {}
//
//void LevelRenderer::render(Level& level)
//{
//	visuals.woodTexture -> bind(TextureSlot::WOOD);
//	for (int z = 0; z < level.getH(); z++)
//	{
//		for (int x = 0; x < level.getW(); x++)
//		{
//			//FLOOR
//			visuals.lightingShader->use();
//			visuals.lightingShader->setFloat("emissiveStrength", 0.0f);
//			glm::mat4 floorModel = glm::mat4(1.0f);
//			floorModel = glm::translate(
//				floorModel,
//				glm::vec3(x + 0.5f, -6.0f, z + 0.5f)
//			);
//			visuals.lightingShader->setMat4("model", floorModel);
//			visuals.cubeMesh->draw(*visuals.lightingShader); 
//
//			//CEILING
//			glm::mat4 ceilingModel = glm::mat4(1.0f);
//			ceilingModel = glm::translate(
//				ceilingModel,
//				glm::vec3(x + 0.5f, 6.0f, z + 0.5f)
//			);
//
//			visuals.lightingShader->setMat4("model", ceilingModel);
//			visuals.cubeMesh->draw(*visuals.lightingShader);
//		}
//	}
//
//	int i = 0; 
//	for (int z = 0; z < level.getH(); z++)
//	{
//		for (int x = 0; x < level.getW(); x++)
//		{
//			if (level.at(x, z) == '#')
//			{
//				visuals.lightingShader->use();
//				visuals.lightingShader->setFloat("emissiveStrength", 0.0f);
//				visuals.woodTexture -> bind(0);
//				glm::mat4 model = glm::mat4(1.0f);
//				model = glm::translate(model, glm::vec3((float)x, 0.0f, (float)z));
//				visuals.lightingShader->setMat4("model", model);
//				visuals.cubeMesh->draw(*visuals.lightingShader);
//			}
//			else if (level.at(x, z) == 'B')
//			{
//				visuals.lightingShader->use();
//				visuals.lightingShader->setVec3("emissiveColor", glm::vec3(1.0f, 0.8f, 0.1f));
//				visuals.lightingShader->setFloat("emissiveStrength", 1.0f);
//				glm::mat4 model = glm::mat4(1.0f);
//				model = glm::translate(model, glm::vec3((float)x, -0.5f, (float)z));
//				glm::vec3 scaleVector = glm::vec3(0.2f, 0.2f, 0.2f);
//				model = glm::scale(model, scaleVector);
//				visuals.lightingShader->setMat4("model", model);
//				visuals.propModel->Draw(*visuals.lightingShader);
//
//				std::string base = "pointLights[" + std::to_string(i) + "]"; 
//				visuals.lightingShader->setVec3(base + ".position", glm::vec3((float)x, 0.0f, (float)z));
//
//				visuals.lightingShader->setVec3(base + ".ambient", 1.0f, 0.8f, 0.1f);
//				visuals.lightingShader->setVec3(base + ".diffuse", 1.0f, 0.8f, 0.1f);
//				visuals.lightingShader->setVec3(base + ".specular", 1.0f, 0.8f, 0.1f);
//
//				visuals.lightingShader->setFloat(base + ".constant", 1.0f);
//				visuals.lightingShader->setFloat(base + ".linear", 0.7f);
//				visuals.lightingShader->setFloat(base + ".quadratic", 1.8f);
//				i++; 
//			}
//			else if (level.at(x, z) == 'M')
//			{
//
//				visuals.animationShader->use();
//
//				auto transforms = visuals.animator->GetFinalBoneMatrices();
//				for (int i = 0; i < transforms.size(); ++i)
//					visuals.animationShader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
//
//				glm::mat4 model = glm::mat4(1.0f);
//				model = glm::translate(model, glm::vec3((float)x, -2.0f, (float)z));
//				//model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
//
//				model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
//				visuals.animationShader->setMat4("model", model);
//				visuals.animationModel->Draw(*visuals.animationShader);
//			}
//		}
//	}
//	visuals.lightingShader->setInt("numPointLights", i);
//}