#include "shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

static std::string loadFile(const char* path)
{
	std::ifstream file(path); 
	std::stringstream ss; 
	ss << file.rdbuf(); 
	return ss.str(); 
}

Shader::Shader(const char* vsPath, const char* fsPath)
{
	std::string vsCode = loadFile(vsPath); 
	std::string fsCode = loadFile(fsPath); 

	const char* vsrc = vsCode.c_str(); 
	const char* fsrc = fsCode.c_str(); 

	unsigned int vs = glCreateShader(GL_VERTEX_SHADER); 
	glShaderSource(vs, 1, &vsrc, nullptr); 
	glCompileShader(vs); 

	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER); 
	glShaderSource(fs, 1, &fsrc, nullptr); 
	glCompileShader(fs);

	id_ = glCreateProgram(); 
	glAttachShader(id_, vs); 
	glAttachShader(id_, fs); 
	glLinkProgram(id_); 

	glDeleteShader(vs); 
	glDeleteShader(fs); 
}

Shader::~Shader()
{
	if (id_ != 0) { glDeleteProgram(id_); }
}

Shader::Shader(Shader&& other) noexcept
{
	id_ = other.id_; 
	other.id_ = 0; 
}

Shader& Shader::operator=(Shader&& other) noexcept 
{
	if (this != &other)
	{
		glDeleteProgram(id_); 
		id_ = other.id_; 
		other.id_ = 0; 
	}

	return *this; 
}

void Shader::use() const
{
	glUseProgram(id_); 
}