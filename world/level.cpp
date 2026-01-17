#include "level.h"
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <algorithm>

namespace {
    std::vector<std::string> levelMap; 
    int H = 0; 
    int W = 0;
}

Level::Level(const std::string& path)
{
    loadFromFile(path); 
}

void Level::loadFromFile(const std::string& path)
{
    std::ifstream file(path); 
    if (!file)
    {
        throw std::runtime_error("Failed to open level"); 
    }
    levelMap.clear(); 
    std::string line; 
    while (std::getline(file, line))
    {
        levelMap.push_back(line); 
    }

    H = levelMap.size();
    W = levelMap[0].size();
    std::cout << W << " " << H << std::endl; 
}


char Level::at(int x, int z) const 
{
    //return '.';
    if (z < 0 || z >= H || x < 0 || x >= W)
        throw std::out_of_range("Level::at out of bounds");
    return levelMap[z][x];
}

void Level::set(int x, int z, char symbol)
{
    levelMap[z][x] = symbol; 
}

bool Level::isWall(float x, float z) const 
{
    if (x < 0 || z < 0 || x >= W || z >= H)
        return true;

    return at(x, z) == '#';
}

int Level::getH()
{
    return H; 
}

int Level::getW()
{
    return W; 
}

glm::vec3 Level::findSpawn() const 
{
    for (int z = 0; z < H; z++)
    {
        for (int x = 0; x < W; x++)
        {
            if (at(x, z) == '.')
                return { x + 0.5f, 1.6f, z + 0.5f };
        }
    }
   

    throw std::runtime_error("No spawn point");
}

bool Level::collides(float x, float z, float radius) const
{
    int minX = static_cast<int>(std::floor(x - radius + 0.5f));
    int maxX = static_cast<int>(std::floor(x + radius + 0.5f));
    int minZ = static_cast<int>(std::floor(z - radius + 0.5f));
    int maxZ = static_cast<int>(std::floor(z + radius + 0.5f));

    for (int currZ = minZ; currZ <= maxZ; currZ++)
    {
        for(int currX = minX; currX <= maxX; currX++)
        {
            if (!isWall(currX, currZ))continue; 
            float tileMinX = static_cast<float>(currX) - 0.5f;
            float tileMaxX = static_cast<float>(currX) + 0.5f;
            float tileMinZ = static_cast<float>(currZ) - 0.5f;
            float tileMaxZ = static_cast<float>(currZ) + 0.5f;

            float clampX = std::clamp(x, tileMinX, tileMaxX);
            float clampZ = std::clamp(z, tileMinZ, tileMaxZ);

            float distX = x - clampX;
            float distZ = z - clampZ;

            if ((distX * distX + distZ * distZ) < (radius * radius)) return true;
        }
    }
    return false; 
}