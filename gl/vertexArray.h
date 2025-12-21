#pragma once
#include <glad/glad.h>

class VertexArray
{
public:
	VertexArray() { glGenVertexArrays(1, &id_); }
	~VertexArray() { glDeleteVertexArrays(1, &id_); }

	//this line below is how c++ copy functions and for copy functions case:
	//	VertexArray b = a; does VertexArray b(a); pass a by reference
	//We dont want VertexArray to be able to do that so its auto deleted.
	VertexArray(const VertexArray&) = delete;
	//line below same case but for a = b -> b.operator=(a);
	VertexArray& operator=(const VertexArray&) = delete;
	void bind() const { glBindVertexArray(id_); }

private:
	unsigned int id_ = 0;
};