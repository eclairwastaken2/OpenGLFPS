#include "material.h"
#include "shader.h"
#include "texture2d.h"

void Material::bind() const
{
	if (texture_)
	{
		texture_->bind(0); 
	}
}

void Material::setTexture(std::shared_ptr<Texture2D> texture)
{
	texture_ = texture; 
}