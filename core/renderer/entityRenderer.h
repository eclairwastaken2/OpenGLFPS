#pragma once
#include <cstdint>

#include <vector>
#include <glm/glm.hpp>

class Camera; 
class Mesh; 
class Model; 
class Material; 
class Animator; 
class Shader; 

class Renderer
{
public:
	Renderer();

	void begin(const Camera& camera);
	void end();
	void submitMesh(
		const Mesh& mesh,
		const Material& material,
		const glm::mat4& transform);

	void submitAnimated(
		const Model& model,
		const Animator& animator,
		const glm::mat4& transform);

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
		const Mesh* mesh;
		const Material* material;
		glm::mat4 transform;
	};

	struct AnimatedDraw
	{
		const Model* model; 
		const Animator* animator; 
		glm::mat4 transform;
	};
	std::vector<ModelBatch> modelBatches_;
	std::vector<MeshBatch> meshBatches_;
	std::vector<AnimatedDraw> animatedDraws_;

	//void renderShadowPass();;
	void renderAnimatedPass();
	void renderMeshPass();
	//void renderModelPass(); 
		
	void drawInstancedBatch(const Mesh& mesh,
		const Material& material,
		const std::vector<glm::mat4>& transforms);

	void drawInstancedBatch(const Model& model,
		const std::vector<glm::mat4>& transforms);

private:
	glm::mat4 view_; 
	glm::mat4 projection_; 
	Shader* staticShader_;
	Shader* animatedShader_;
	//Shader* shadowShader_;
};