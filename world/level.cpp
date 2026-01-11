#include "level.h"
#include <stdexcept>

namespace {
    constexpr const char levelMap[] =
        "##########"
        "#.B......#"
        "#..##....#"
        "#........#"
        "#....##..#"
        "#........#"
        "#..##....#"
        "#........#"
        "#........#"
        "##########";
}

char Level::at(int x, int z) const 
{
    return levelMap[z * W + x];
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