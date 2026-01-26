#pragma once
#include <cstdint>
#include "animation/animator.h"
#include "animation/model_animation.h"
#include "gl/mesh.h"
#include "glm/vec3.hpp"

class Entity
{
public:
	Entity(const TexturedModel& model_, const glm::vec3& position_, const glm::vec3& _rotation, float scale_);

	const TexturedModel& getModel() const; 
	void setModel(const TexturedModel& model); 
	void glm::vec3& getPosition() const; 
	void setPosition(const glm::vec3& position); 
	const glm::vec3& getRotation() const; 
	void setRotation(const glm::vec3& rotation); 
	float getScale() const;
	void scale(float scale_); 

	void translate(float dx, float dy, float dz); 
	void rotate(float dx, float dy, float dz); 
	float getTextureXOffset() const; 
	float getTextureYOffset() const; 

protected:
	TexturedModel model_; 
	glm::vec3 position_; 
	glm::vec3 rotation_; 
	float scale_; 
	int textureIndex_ = 0;

};