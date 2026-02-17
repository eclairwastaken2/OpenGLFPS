#pragma once
#include <cstdint>

#include <vector>
#include "gl/model.h"

#include <glm/glm.hpp>

class Camera; 
class Mesh; 
class Model; 
class Material; 
//class Animator; 
class Shader; 

class Renderer
{
public:
	Renderer();

	void begin(const Camera& camera);
	void end();
	void submitMesh(std::shared_ptr<Mesh> mesh,
		const glm::mat4& transform);

	//void submitAnimated(
	//	const Model& model,
	//	const Animator& animator,
	//	const glm::mat4& transform);

	void submitModel(
		const Model& model,
		const glm::mat4& transform);


private:
	struct ModelBatch
	{
		const Model* model;
		glm::mat4 transform;
	};

	struct MeshBatch
	{
		std::shared_ptr<Mesh> mesh;
		glm::mat4 transform;
	};

	//struct AnimatedDraw
	//{
	//	const Model* model; 
	//	const Animator* animator; 
	//	glm::mat4 transform;
	//};
	std::vector<ModelBatch> modelBatches_;
	std::vector<MeshBatch> meshBatches_;
	//std::vector<AnimatedDraw> animatedDraws_;

	//void renderShadowPass();;
	void renderAnimatedPass();
	void renderMeshPass();
	//void renderModelPass(); 
		
	void drawInstancedBatch(const Mesh& mesh,
		const std::vector<glm::mat4>& transforms);

	void drawInstancedBatch(const Model& model,
		const std::vector<glm::mat4>& transforms);

	void applyFrameUniforms(Shader& shader);

private:
	glm::mat4 view_; 
	glm::mat4 projection_; 
	Shader* staticShader_;
	Shader* animatedShader_;
	Camera camera_; 
	//Shader* shadowShader_;
};