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

TextureManager& TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}

//void TextureManager::createTexture(const std::string& fileLocation, const std::string& nimi)
//{
//    TexturePtr t(new Texture());
//    t->create(fileLocation);
//    tekstuurit_.push_back(std::make_pair(nimi,std::move(t)));
//}
//
//Texture* TextureManager::getTexture(const std::string& nimi) const
//{
//    for (const auto& obj : tekstuurit_)
//    {
//        if (std::get<0>(obj) == nimi)
//        {
//            return std::get<1>(obj).get();
//        }
//    }
//    throw std::runtime_error("TextureManager::createTexture -> ei loytynyt tekstuuria nimella '" + nimi + "'.");
//}


