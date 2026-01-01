#pragma once
#include "../gl/mesh.h"
#include "../gl/texture2d.h"

struct DrawCommand
{
	Mesh* mesh; 
	Texture2D* texture; 
	glm::mat4 model;
};