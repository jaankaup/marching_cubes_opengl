#ifndef SHADER_H
#define SHADER_H
#include <iostream>
#include <vector>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "misc.h"

/** A simple shader class with basic shader operations. */
class Shader
{
	public:
		Shader();
		~Shader();

		void build(const std::vector<std::string>& sources);
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


	private:

		GLuint pId = 0;

    std::string loadSource(const std::string& fileLoc);
    GLuint getShaderType(const std::string& fileLoc);
    void compileShader(const GLuint id, const std::string& source);
    bool checkShader(GLuint obj);

        //std::vector<GLuint> createShaderObjects(const std::vector<std::string>& shaderSources) const;
};


#endif // SHADER_H
