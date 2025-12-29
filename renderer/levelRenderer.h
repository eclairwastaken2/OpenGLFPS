#pragma once
#include "../world/level.h"
#include "../gl/shader.h"
#include "levelVisuals.h"

class LevelRenderer
{
public:
	explicit LevelRenderer(const LevelVisuals& visuals); 

	void render(Level& level, Shader& shader); 

private:
	const LevelVisuals& visuals; 
};