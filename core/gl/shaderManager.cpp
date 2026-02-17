#include "shaderManager.h"

std::map<std::string, std::shared_ptr<Shader>>
ShaderManager::shaders_;

void ShaderManager::initialize()
{
    shaders_["light"] = std::make_shared<Shader>(
        "Assets/shaders/light_caster.vs",
        "Assets/shaders/light_caster.fs"
    );
}

std::shared_ptr<Shader>
ShaderManager::get(const std::string& name)
{
    return shaders_.at(name);
}