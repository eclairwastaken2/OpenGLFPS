#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <string>

class Shader; 
class Texture2D; 

class Material
{
public:
	void bind() const; 

	void setTexture(std::shared_ptr<Texture2D> texture); 

private:
	std::shared_ptr<Texture2D> texture_; 
};