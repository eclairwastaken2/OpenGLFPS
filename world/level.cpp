#include "level.h"
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>

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
    for (int i = 0; i < H; i++)for (int j = 0; j < W; j++)std::cout << levelMap[i][j] << std::endl; 
}


char Level::at(int x, int z) const 
{
    //return '.';
    if (z < 0 || z >= H || x < 0 || x >= W)
        throw std::out_of_range("Level::at out of bounds");
    return levelMap[z][x];
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