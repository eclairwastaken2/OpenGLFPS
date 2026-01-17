#pragma once
#include "level.h"
#include <glm/glm.hpp>
#include <cmath>

class Player
{
public:
	void tryCollect(Level& level) const
	{
		int x = static_cast<int>(std::floor(position.x)); 
		int z = static_cast<int>(std::floor(position.z)); 

		if (level.at(x, z) == 'B')
		{
			level.set(x, z, '.'); 
		}
	}

	void setPosition(glm::vec3 newPosition) 
	{
		position = newPosition; 
	}

	glm::vec3 getPosition()
	{
		return position; 
	}
private:
	glm::vec3 position; 
	float radius = 0.3f; 
};