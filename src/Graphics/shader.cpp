#include <fstream>
#include <streambuf>
#include <stdexcept>
#include "shader.h"
#include "../Utils/log.h"


Shader::Shader()
{
  pId = glCreateProgram();
  if (pId == 0)
  {
    Log::getError().log("%","Shader::Shader. Failed to create program.");
  }
}

Shader::~Shader()
{
  if (pId != 0) glDeleteProgram(pId);
}

void Shader::build(const std::vector<std::string>& sources)
{

}

void Shader::bind()
{

}

std::string Shader::loadSource(const std::string& fileLoc)
{
    std::ifstream file(fileLoc);

    if (!file.is_open())
    {
      Log::getError().log("Shader::loadSource(%): Failed to load source.","fileLoc");
      throw std::runtime_error("Shader::loadSource: counld'n open file '" + fileLoc + "'.");
    }

    std::string str;
    file.seekg(0, std::ios::end);
    str.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    str.assign((std::istreambuf_iterator<char>(file)),
               std::istreambuf_iterator<char>());

    return str;
}


GLuint Shader::getShaderType(const std::string& fileLoc)
{
  static const auto endsWith = [](const std::string& str, const std::string& postFix)
  {
    if (str.length() >= postFix.length()) {
      return (0 == str.compare(str.length() - postFix.length(), postFix.length(), postFix));
    } else {
      return false;
    } 
  };


    /* Supported shader object source code types. */
    static std::array<std::string, 6> fileSuffixes = {".vert",
                                                      ".gtcs",
                                                      ".gtes",
                                                      ".geom",
                                                      ".frag",
                                                      ".comp"
                                                     };

    /* Corrresponding shader object types in opengl. */
    static std::array<GLuint, 5> GLStageEnums = {GL_VERTEX_SHADER,
                                                 GL_TESS_CONTROL_SHADER,
                                                 GL_TESS_EVALUATION_SHADER,
                                                 GL_GEOMETRY_SHADER,
                                                 GL_FRAGMENT_SHADER,
                                                 //GL_COMPUTE_SHADER
                                                };

    for (unsigned int i=0 ; i<fileSuffixes.size() ; i++)
    {
        if (endsWith(fileLoc,fileSuffixes[i])) return GLStageEnums[i];
    }
    Log::getError().log("Shader::getShaderType(%): Failed to determine the shader type.",fileLoc);
    return 0;
}

void Shader::compileShader(const GLuint id, const std::string& source)
{
    // Transform c++ string to C string.
    const char* src_C = source.c_str();
    // Push source to shader object managed by opengl.
    glShaderSource(id, 1, &src_C, NULL);
    // return shader object handle.
    glCompileShader(id);
}

bool Shader::checkShader(GLuint obj)
{
    /* We are checking linking status by default. */
    GLenum pname = GL_COMPILE_STATUS;

    int status;
    int errorLength;
    glGetShaderiv(obj, pname, &status);

    if (status == GL_FALSE)
    {
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &errorLength);
        GLchar *errorMessage = new GLchar[errorLength];
        glGetShaderInfoLog(obj, errorLength, NULL, errorMessage);
        Log::getError().log("Shader::getShaderType(%): Shader object compilation failed: %", errorMessage);
        delete[] errorMessage;
        return false;
    }
    return true;
}
