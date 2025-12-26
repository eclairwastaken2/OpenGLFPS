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

    // utility uniform functions
 // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(id_, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(id_, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(id_, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private: 
	GLuint id_ = 0; 
};