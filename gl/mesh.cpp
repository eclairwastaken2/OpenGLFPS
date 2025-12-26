#include "mesh.h"
#include <glad/glad.h>

//“Before the constructor body runs, construct the member vbo using the argument GL_ARRAY_BUFFER.”
Mesh::Mesh(const std::vector<float>& vertices) : vbo(GL_ARRAY_BUFFER)
{
	vertexCount = static_cast<int>(vertices.size() / 8); 

	vao.bind(); 
	vbo.bind(); 

	glBufferData(GL_ARRAY_BUFFER,
		vertices.size() * sizeof(float),
		vertices.data(),
		GL_STATIC_DRAW); 

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, vbo.getId());
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Mesh::draw() const
{
	vao.bind(); 
	glDrawArrays(GL_TRIANGLES, 0, vertexCount); 
}