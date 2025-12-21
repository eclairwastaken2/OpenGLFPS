#pragma once
#include <glm/glm.hpp>

class Level
{
public: 

	bool isWall(float x, float z) const; 
	char at(int x, int z) const;
	glm::vec3 findSpawn() const; 

	int getW(); 
	int getH(); 
protected:
	static constexpr int W = 10;
	static constexpr int H = 10;
};