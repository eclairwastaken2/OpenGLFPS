#pragma once
#include "glad/glad.h"

class Texture2D
{
public:
	Texture2D() { glGenTextures(1, &id_); }
	~Texture2D() { glDeleteTextures(1, &id_); }

	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;

	void bind(GLuint unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, id_);
	}

	void setWrap(GLenum s, GLenum t) const
	{
		bind(0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
	}

	void setFilter(GLenum min, GLenum mag) const
	{
		bind(0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	}

	void upload(int width, int height, GLenum format, const unsigned char* data) const
	{
		bind(0);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
			format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

private:
	unsigned int id_ = 0;
};