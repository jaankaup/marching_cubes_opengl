#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

//include <iostream>
#include <vector>
#include <tuple>
#include <stdexcept>
//#include <GL/glew.h>
//#include <SOIL.h>
#include "shader.h"
#include "../Utils/log.h"

class Shader;

class ShaderManager
{
  /// Creates and return a reference to the ShaderManager object.
  public:
    static ShaderManager& getInstance();
  
    Shader createShader(const std::string& name);
  
    /// Get shader by its name. throws runtime_expecton if texture is not found.  
    Shader getShaderByName(const std::string& name) const;
  
  private:
    ShaderManager();
    ~ShaderManager();
  
    /// Name:Shader pairs.
    std::vector<std::tuple<const std::string, Shader>> pShaders;
};

#endif // SHADERMANAGER_H
