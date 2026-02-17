#pragma once
#include "glad/glad.h"
#include <string>
#include <stdexcept>
#include "stb/stb_image.h"

class Texture2D
{
public:
	Texture2D() { glGenTextures(1, &id_); }
    ~Texture2D()
    {
        if (id_ != 0)
            glDeleteTextures(1, &id_);
    }

	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&& other) noexcept
        : id_(other.id_)
    {
        other.id_ = 0;
    }

    Texture2D& operator=(Texture2D&& other) noexcept
    {
        if (this != &other)
        {
            glDeleteTextures(1, &id_);

            id_ = other.id_;
            other.id_ = 0;
        }
        return *this;
    }

	void bind(GLuint unit = 0) const
	{
        if (id_ == 0)
            return; // prevent crash

		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, id_);
	}

	void setWrap(GLenum s, GLenum t) const
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
	}

	void setFilter(GLenum min, GLenum mag) const
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	}

	void upload(int width, int height, GLenum format, const unsigned char* data) const
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
			format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	class Builder; 

private:
	unsigned int id_ = 0;
};

class Texture2D::Builder
{
public:
    Builder& wrap(GLenum s, GLenum t)
    {
        wrapS = s;
        wrapT = t;
        return *this;
    }

    Builder& filter(GLenum min, GLenum mag)
    {
        minFilter = min;
        magFilter = mag;
        return *this;
    }

    Texture2D fromFile(const std::string& path)
    {
        Texture2D tex;
        tex.bind();

        tex.setWrap(wrapS, wrapT);
        tex.setFilter(minFilter, magFilter);

        int w, h, ch;
        unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, 0);
        if (!data)
            throw std::runtime_error("Failed to load texture: " + path);

        GLenum format = (ch == 4) ? GL_RGBA : GL_RGB;
        tex.upload(w, h, format, data);
        stbi_image_free(data);

        return tex; 
    }

private:
    GLenum wrapS = GL_REPEAT;
    GLenum wrapT = GL_REPEAT;
    GLenum minFilter = GL_LINEAR;
    GLenum magFilter = GL_LINEAR;
};