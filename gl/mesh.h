#pragma once
#include <vector>
#include "vertexArray.h"
#include "buffer.h"

class Mesh
{
public:
	Mesh(const std::vector<float>& vertices); 
	void draw() const; 

private:
	VertexArray vao; 
	Buffer vbo; 
	int vertexCount; 
};