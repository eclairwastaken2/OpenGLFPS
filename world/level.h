#pragma once
#include <glm/glm.hpp>
#include <string>

class Level
{
public: 
	Level(const std::string& path); 
	bool isWall(float x, float z) const; 
	char at(int x, int z) const;
	glm::vec3 findSpawn() const; 

	int getW(); 
	int getH(); 
protected:
	void loadFromFile(const std::string& path); 
	std::vector<std::string> levelMap; 
	int W = 0;
	int H = 0;
};