#pragma once

#include <memory>

class Shader; 
class Texture2D; 

class Material
{
public:
	Material(std::shared_ptr<Shader> shader);
	void bind() const; 
	const Shader& getShader() const;
	void setDiffuse(std::shared_ptr<Texture2D> texture); 
	void setSpecular(std::shared_ptr<Texture2D> texture);
	void setNormal(std::shared_ptr<Texture2D> texture);
	void setHeight(std::shared_ptr<Texture2D> texture);

private:
	std::shared_ptr<Texture2D> diffuse_; 
	std::shared_ptr<Texture2D> specular_;
	std::shared_ptr<Texture2D> normal_;
	std::shared_ptr<Texture2D> height_;
	std::shared_ptr<Shader> shader_;

};