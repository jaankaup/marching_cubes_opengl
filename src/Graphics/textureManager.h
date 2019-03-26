#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

//include <iostream>
#include <vector>
#include <tuple>
//#include <GL/glew.h>
//#include <SOIL.h>
#include "texture.h"
#include "../Utils/log.h"

class Texture;

class TextureManager
{
  /// Creates and return a reference to the TextureManager object.
  public:
  static TextureManager& getInstance();
  /// Creates a texture.
//  void createTexture(const std::string& fileLocation, const std::string& nimi);
  /// Palauttaa pointerinTextureen.
//  Texture* getTexture(const std::string& nimi) const;

  /// Creates a 2D textrure.
  Texture create2D(const std::string& name);

  /// Creates a 3D textrure.
  Texture create3D(const std::string& name);

  private:
  TextureManager();
  ~TextureManager();

  /// Name:Texture pairs.
  std::vector<std::tuple<const std::string, Texture>> pTextures;
};

#endif // TEXTUREMANAGER_H
