#include "shaderManager.h"

ShaderManager::ShaderManager()
{

}

ShaderManager::~ShaderManager()
{
   for (const auto& shaderData : pShaders)
   {
     std::get<1>(shaderData).dispose();
   }
}

Shader* ShaderManager::createShader(const std::string& name)
{
  Shader s;
  s.init();
  auto shaderData = std::make_tuple(name,s);
  pShaders.push_back(shaderData);
  return getShaderByName(name);
}

ShaderManager& ShaderManager::getInstance()
{
    static ShaderManager instance;
    return instance;
}

Shader* ShaderManager::getShaderByName(const std::string& name)
{
    for (const auto& obj : pShaders)
    {
        if (std::get<0>(obj) == name)
        {
            return const_cast<Shader*>(&std::get<1>(obj));
        }
    }
    throw std::runtime_error("ShaderManager::getShaderByName(" + name + "). No such shader.");
}
