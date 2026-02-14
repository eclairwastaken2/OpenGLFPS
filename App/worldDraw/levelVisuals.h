#pragma once

#include "gl/mesh.h"
#include "animation/model_animation.h"
#include "gl/texture2d.h"
#include "animation/animation.h"
#include "animation/animator.h"
#include "gl/shader.h"

struct LevelVisuals
{
	Mesh* cubeMesh; 
	Model* propModel;
	Model* animationModel; 
	Animation* animation;
	Animator* animator; 


	Shader* animationShader; 
	Shader* lightingShader; 
	Texture2D* woodTexture; 
};

enum TextureSlot : unsigned
{
	WOOD = 0
};