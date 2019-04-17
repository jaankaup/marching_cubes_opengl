#include <fstream>
#include <streambuf>
#include <stdexcept>
#include "shader.h"
#include "../Utils/log.h"


Shader::Shader()
{
}

Shader::~Shader()
{
}

void Shader::dispose() const
{
  Log::getDebug().log("Deleting shader program: %", std::to_string(pId));
  if (pId != 0) glDeleteProgram(pId);
}

void Shader::init()
{
  pId = glCreateProgram();

  Log::getDebug().log("Created shader program: %", std::to_string(pId));

  if (pId == 0)
  {
    Log::getError().log("%","Shader::Shader. Failed to create program.");
  }
}

void Shader::setFeedback(const bool feedback, const std::string& feedbackVarying)
{
  pFeedback = feedback;
  pFeedbackVarying = feedbackVarying;
}

void Shader::build(const std::vector<std::string>& sources)
{
    using ShaderObjData = struct{
        GLenum shaderType;
        GLuint shaderObj;
        std::string sourceCode;
    };

    /* A typedef for shader object data: shader object type and source code. */
    using SOD = std::vector<ShaderObjData>;

    /* A container for shader objects type and source code. */
    SOD sod;

    /* We iterate trought all source codes and extract shader object types and sourcecodes. */
    for (const auto& location : sources)
    {
        Log::getDebug().log("Creating shader from file: %", location);

        /* Solve the type of shader object from file extension. */
        GLenum shaderType = getShaderType(location);
        if (shaderType == 0)
        {
            Log::getError().log("Shader::build: Could'n solve the shader object type from file '%'.",location);
            Log::getError().log("Shader::build: file extension must be one the following: [%].",".vert .gtcs .gtes .geom .frag");
            // TODO: exception
        }

        ShaderObjData shaderObjectData;
        shaderObjectData.shaderType = shaderType;
        shaderObjectData.shaderObj = glCreateShader(shaderType);
        shaderObjectData.sourceCode = loadSource(location);
        /* Put the source type and source code to sod. */
        sod.push_back(shaderObjectData);
    }

    for (const auto& object : sod)
    {
        compileShader(object.shaderObj, object.sourceCode);
        if (!checkShader(object.shaderObj))
            {
            Log::getError().log("Shader::build: couldn't build shader object from source:\n%",object.sourceCode);
            std::runtime_error("Shader::build: An error occurred while compiling a shader object.");
            }
    }

    /* Attach shader objects to the program. */
    for (const auto& object : sod)
    {
        glAttachShader(pId, object.shaderObj);
//        Log::getDebug().log("Shader type is geometry: %",object.shaderType == GL_GEOMETRY_SHADER);
    }
    if (pFeedback)
    {
      const GLchar* feedbackVaryings[] = {"outputCase"}; //pFeedbackVarying.c_str()}; 
      Log::getDebug().log("feedback varying: %",feedbackVaryings[0]);
      Log::getDebug().log("pId: %",std::to_string(pId));
      glTransformFeedbackVaryings(pId, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);
    }

    GLenum pname = GL_LINK_STATUS;

    Log::getDebug().log("Linking shader.");

    glLinkProgram(pId);

    Log::getDebug().log("Linked shader succeed.");

    int status;
    int errorLength;

    Log::getDebug().log("Checking shader for errors.");

//    glGetShaderiv(pId, pname, &status);
    glGetProgramiv(pId, pname, &status);

    if (status == GL_FALSE)
    {
        glGetProgramiv(pId, GL_INFO_LOG_LENGTH, &errorLength);
        GLchar *errorMessage = new GLchar[errorLength];
        glGetProgramInfoLog(pId, errorLength, NULL, errorMessage);
        Log::getError().log("Shader program creation failed: %", errorMessage);
        delete[] errorMessage;
    }

    //for (const auto object : sod)
    //{
    //    glDeleteShader(object.shaderObj);
    //}
    Log::getDebug().log("Shader program created succesfully.");
}

void Shader::bind() const
{
//  Log::getDebug().log("Binding shader %.",std::to_string(pId));
  glUseProgram(pId);
}

void Shader::setUniform(const std::string& name, int value) const
{
    GLint loc = glGetUniformLocation(pId, name.c_str());
    //if (loc == -1) logDebug.log("setUniform(%) ei loydy!", name);
    glUniform1i(loc, value);
}

void Shader::setUniform(const std::string& name, float value) const
{
    GLint loc = glGetUniformLocation(pId, name.c_str());
    //if (loc == -1) logDebug.log("setUniform(%) ei loydy!", name);
    glUniform1f(loc, value);
}

void Shader::setUniform(const std::string& name, const glm::vec3& value) const
{
    GLint loc = glGetUniformLocation(pId, name.c_str());
    //if (loc == -1) logDebug.log("setUniform(%) ei loydy!", name);
    glUniform3fv(loc, 1,  glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat3& value) const
{
    GLint loc = glGetUniformLocation(pId, name.c_str());
    //if (loc == -1) logDebug.log("setUniform(%) ei loydy!", name);
    glUniformMatrix3fv(loc, 1, GL_FALSE,  glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::vec4& value) const
{
    GLint loc = glGetUniformLocation(pId, name.c_str());
    //if (loc == -1) logDebug.log("setUniform(%) ei loydy!", name);
    glUniform4fv(loc, 1,  glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat4& value) const
{
    GLint loc = glGetUniformLocation(pId, name.c_str());
    //if (loc == -1) logDebug.log("setUniform(%) ei loydy!", name);
    glUniformMatrix4fv(loc, 1, GL_FALSE,  glm::value_ptr(value));
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
    static std::array<std::string, 5> fileSuffixes = {".vert",
                                                      ".gtcs",
                                                      ".gtes",
                                                      ".geom",
                                                      ".frag"
//                                                      ".comp"
                                                     };

    /* Corrresponding shader object types in opengl. */
    static std::array<GLuint, 5> GLStageEnums = {GL_VERTEX_SHADER,
                                                 GL_TESS_CONTROL_SHADER,
                                                 GL_TESS_EVALUATION_SHADER,
                                                 GL_GEOMETRY_SHADER, // 0x8dd9,
                                                 GL_FRAGMENT_SHADER
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
    Log::getDebug().log("Shader::compileShaer(): Compiling shader: %", std::to_string(id));
    // Transform c++ string to C string.
    const char* src_C = source.c_str();
    // Push source to shader object managed by opengl.
    glShaderSource(id, 1, &src_C, NULL);
    // return shader object handle.
    glCompileShader(id);
}

bool Shader::checkShader(GLuint obj)
{
    Log::getDebug().log("Shader::checkShader(): Checking shader: %", std::to_string(obj));
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
    Log::getDebug().log("Shader::checkShader(): Shader checked successfully: %", std::to_string(obj));
    return true;
}
