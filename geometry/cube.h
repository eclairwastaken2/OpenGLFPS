#pragma once
#include <vector>
#include "../gl/mesh.h"

inline Mesh MakeCube()
{
    std::vector<Vertex> vertices(24);
    std::vector<unsigned int> indices;

    // positions, normals, texcoords
    glm::vec3 positions[] = {
        {-0.5f, -2.0f,  0.5f}, {0.5f, -2.0f,  0.5f},
        {0.5f,  4.0f,  0.5f}, {-0.5f,  4.0f,  0.5f},
        { 0.5f, -2.0f, -0.5f}, {-0.5f, -2.0f, -0.5f},
        {-0.5f,  4.0f, -0.5f}, { 0.5f,  4.0f, -0.5f},
        {-0.5f, -2.0f, -0.5f}, {-0.5f, -2.0f,  0.5f},
        {-0.5f,  4.0f,  0.5f}, {-0.5f,  4.0f, -0.5f},
        { 0.5f, -2.0f,  0.5f}, { 0.5f, -2.0f, -0.5f},
        { 0.5f,  4.0f, -0.5f}, { 0.5f,  4.0f,  0.5f},
        {-0.5f,  4.0f,  0.5f}, {0.5f,  4.0f,  0.5f},
        {0.5f,  4.0f, -0.5f}, {-0.5f,  4.0f, -0.5f},
        {-0.5f, -2.0f, -0.5f}, {0.5f, -2.0f, -0.5f},
        {0.5f, -2.0f,  0.5f}, {-0.5f, -2.0f,  0.5f}
    };

    glm::vec3 normals[] = {
        {0,0,1},{0,0,1},{0,0,1},{0,0,1},
        {0,0,-1},{0,0,-1},{0,0,-1},{0,0,-1},
        {-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},
        {1,0,0},{1,0,0},{1,0,0},{1,0,0},
        {0,1,0},{0,1,0},{0,1,0},{0,1,0},
        {0,-1,0},{0,-1,0},{0,-1,0},{0,-1,0}
    };

    glm::vec2 uvs[] = {
        {0,0},{1,0},{1,1},{0,1},
        {0,0},{1,0},{1,1},{0,1},
        {0,0},{1,0},{1,1},{0,1},
        {0,0},{1,0},{1,1},{0,1},
        {0,0},{1,0},{1,1},{0,1},
        {0,0},{1,0},{1,1},{0,1}
    };

    for (int i = 0; i < 24; i++)
    {
        vertices[i].Position = positions[i];
        vertices[i].Normal = normals[i];
        vertices[i].TexCoords = uvs[i];
    }

    // 6 faces × 2 triangles
    for (int face = 0; face < 6; face++)
    {
        unsigned int start = face * 4;
        indices.insert(indices.end(), {
            start + 0, start + 1, start + 2,
            start + 2, start + 3, start + 0
            });
    }

    return Mesh(vertices, indices, {});
}