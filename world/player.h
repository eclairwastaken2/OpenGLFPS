#pragma once
#include "level.h"
#include <glm/glm.hpp>
#include <cmath>

class Player
{
public:
	void tryCollect(Level& level) const
	{
		int px = static_cast<int>(std::floor(position.x)); 
		int pz = static_cast<int>(std::floor(position.z)); 

		for (int dz = -1; dz <= 1; dz++)
		{
			for (int dx = -1; dx <= 1; dx++)
			{
				int x = px + dx; 
				int z = pz + dz;

				if (!level.inBounds(x, z)) continue; 

				if (level.at(x, z) == 'B')
				{
					glm::vec3 gemPos(x, position.y, z); 

					float dist = glm::length(position - gemPos); 

					if (dist <= radius)
					{
						level.set(x, z, '.'); 
				
					}
				}
			}
		}

		//if (level.at(x, z) == 'B')
		//{
		//	level.set(x, z, '.'); 
		//}
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
	float radius = 0.6f; 
};