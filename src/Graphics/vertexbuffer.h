#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "vertexAttributes.h"

class Vertexbuffer
{
	public:
		Vertexbuffer();
		~Vertexbuffer();

    void init();
    void bind() const;
    void addData(const void* data, unsigned int size) const;
    void createExampleCube();
    GLuint getHandle() const;

	private:
    GLuint pId = 0;
    GLuint pVAO = 0;
    GLenum pTarget;
    GLenum pUsage;
    std::vector<float> pData;

};


#endif // VERTEXBUFFER_H
