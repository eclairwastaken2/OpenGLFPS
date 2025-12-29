#pragma once

#include "../gl/mesh.h"
#include "../gl/model.h"
#include "../gl/texture2d.h"

struct LevelVisuals
{
	Mesh* cubeMesh; 
	Model* propModel; 

	Texture2D* woodTexture; 
};

namespace TextureSlot
{
	unsigned WOOD = 0;
};