#include "mesh.h"
#include <glad/glad.h>

Mesh::Mesh(const std::vector<float>& vertices) : vbo(GL_ARRAY_BUFFER)
{
	vertexCount = static_cast<int>(vertices.size() / 5); 

	vao.bind(); 
	vbo.bind(); 

	glBufferData(GL_ARRAY_BUFFER,
		vertices.size() * sizeof(float),
		vertices.data(),
		GL_STATIC_DRAW); 

	//position
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(float),
		(void*)0
	); 
	glEnableVertexAttribArray(0); 

	//texcoords
	glVertexAttribPointer(
		1, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(float),
		(void*)(3 * sizeof(float))
	);
	glEnableVertexAttribArray(1); 
}

void Mesh::draw() const
{
	vao.bind(); 
	glDrawArrays(GL_TRIANGLES, 0, vertexCount); 
}