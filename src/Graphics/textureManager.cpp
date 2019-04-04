#include "textureManager.h"

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{
   for (const auto& texData : pTextures)
   {
     std::get<1>(texData).dispose();
   }
}

Texture TextureManager::create2D(const std::string& name)
{
  Texture t;
  t.init(TextureType::d2);
  auto textureData = std::make_tuple(name,t);
  pTextures.push_back(textureData);
  return t;
}

Texture TextureManager::create3D(const std::string& name)
{
  Texture t;
  t.init(TextureType::d3);
  auto textureData = std::make_tuple(name,t);
  pTextures.push_back(textureData);
  return t;
}

Texture TextureManager::create1D(const std::string& name)
{
  Texture t;
  t.init(TextureType::d1);
  auto textureData = std::make_tuple(name,t);
  pTextures.push_back(textureData);
  return t;
}

TextureManager& TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}

Texture TextureManager::getTextureByName(const std::string& name) const
{
    for (const auto& obj : pTextures)
    {
        if (std::get<0>(obj) == name)
        {
            return std::get<1>(obj);
        }
    }
    throw std::runtime_error("TextureManager::getTextureByName(" + name + "). No such texture.");
}

