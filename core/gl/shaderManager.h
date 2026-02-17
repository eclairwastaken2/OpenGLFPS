#pragma once
#include <memory>
#include <map>
#include <string>
#include "shader.h"

class ShaderManager
{
public:
	static void initialize(); 
	static std::shared_ptr<Shader> get(const std::string& name); 

private:
	static std::map<std::string, std::shared_ptr<Shader>> shaders_;
};