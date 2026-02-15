#include "material.h"
#include "shader.h"
#include "texture2d.h"

Material::Material(std::shared_ptr<Shader> shader)
    : shader_(std::move(shader))
{
}

const Shader& Material::getShader() const
{
    return *shader_;
}

void Material::bind() const
{
    int slot = 0;
    shader_->use();

    if (diffuse_)
    {
        diffuse_->bind(slot);
        shader_->setInt("material.diffuse", slot++);
    }

    if (specular_)
    {
        specular_->bind(slot);
        shader_->setInt("material.specular", slot++);
    }

    if (normal_)
    {
        normal_->bind(slot);
        shader_->setInt("material.normal", slot++);
    }

    if (height_)
    {
        height_->bind(slot);
        shader_->setInt("material.height", slot++);
    }
}

void Material::setDiffuse(std::shared_ptr<Texture2D> t) { diffuse_ = t; }
void Material::setSpecular(std::shared_ptr<Texture2D> t) { specular_ = t; }
void Material::setNormal(std::shared_ptr<Texture2D> t) { normal_ = t; }
void Material::setHeight(std::shared_ptr<Texture2D> t) { height_ = t; }
