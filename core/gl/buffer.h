#pragma once
#include <glad/glad.h>

class Buffer {
public:
	explicit Buffer(GLenum type) : type_(type)
	{
		glGenBuffers(1, &id_);
	}

	~Buffer() { glDeleteBuffers(1, &id_); }

	Buffer(const Buffer&) = delete;
	Buffer& operator=(const Buffer&) = delete;

	void bind() const { glBindBuffer(type_, id_); }
	int getId() const { return id_; }

private:
	unsigned int id_ = 0;
	GLenum type_;
};