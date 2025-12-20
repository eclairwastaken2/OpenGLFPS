#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath); 

	~Shader();

	Shader(const Shader&) = delete;  
	Shader& operator=(const Shader&) = delete; 

	Shader(Shader&& other) noexcept; //move constructor
	Shader& operator=(Shader&& other) noexcept; //move assignment

	//move constructor allows this:
	//Shader::Shader(Shader&& other) noexcept
	//{
	//	m_id = other.m_id;   // steal the resource
	//	other.m_id = 0;      // neuter the old object
	//}
	//Used when creating a new object from a temporary.
	//Shader a = makeShader();          // return value
	//Shader b = Shader("v", "f");      // temporary
	//vector.push_back(Shader("v", "f"));// container insert
	//move assignment allows this:
	//Used when replacing an existing object.
	//	Examples :
	//	Shader a("v1", "f1");
	//Shader b("v2", "f2");
	//a = std::move(b);   // move assignment
	//Key difference :
	//a already exists
	//	must clean up a first
	//	then steal from b

	void use() const; 

	void setInt(const std::string& name, int value) const; 

	void setMat4(const std::string& name, const glm::mat4& mat)const; 

private: 
	GLuint id_ = 0; 
};