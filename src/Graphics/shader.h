#ifndef SHADER_H
#define SHADER_H
#include <iostream>
#include <vector>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderManager.h"

/** A simple shader class with basic shader operations. */
class Shader
{
    /// TextureManager object has access to all Texture member functions.
    /// TextureManager creates and destroys textures objects.
    friend class ShaderManager;

	public:

		~Shader();
		void build(const std::vector<std::string>& sources,const bool triangulate);
    void buildDensity(const std::vector<std::string>& sources);
		void bind() const;

    void setUniform(const std::string& name, int value) const;
    void setUniform(const std::string& name, float value) const;
    void setUniform(const std::string& name, const glm::vec3& value) const;
    void setUniform(const std::string& name, const glm::vec4& value) const;
    void setUniform(const std::string& name, const glm::mat3& value) const;
    void setUniform(const std::string& name, const glm::mat4& value) const;
        //void SetUniform(const std::string& name, int count, const glm::vec4* values);
        //void SetUniform(const std::string& name, int count, const glm::mat4* values);
//        GLuint getID() const;
    void setFeedback(const bool feedback, const std::string& feedbackVarying);


	private:

		GLuint pId = 0;
		bool pFeedback = false;
    std::string pFeedbackVarying;

		Shader();

    std::string loadSource(const std::string& fileLoc);
    GLuint getShaderType(const std::string& fileLoc);
    void compileShader(const GLuint id, const std::string& source);
    bool checkShader(GLuint obj);

    void init();

    /// Releases the shader object.
    void dispose() const;
};


#endif // SHADER_H
