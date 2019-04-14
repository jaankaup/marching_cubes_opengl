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

    // Copying causes problems because destructor deletes the buffers.
    Vertexbuffer& operator=(const Vertexbuffer&) = delete;
    Vertexbuffer(const Vertexbuffer&) = delete;

    // Use move instead.
    Vertexbuffer(Vertexbuffer&&) = default;
    Vertexbuffer& operator=(Vertexbuffer&&) = default;

    void init();
    void bind() const;
    void addData(const void* data, unsigned int size, const std::vector<std::string>& types) const;
    void createExampleCube();
    int createExamplePoints(const int dimensionX, const int dimensionY, const int dimensionZ);
    int createExamplePointsTier2(const int dimensionX, const int dimensionY, const int dimensionZ);
    GLuint getHandle() const;
    GLuint getVAO() const;

	private:
    GLuint pId = 0;
    GLuint pVAO = 0;
    GLenum pTarget;
    GLenum pUsage;
    std::vector<float> pData;

};


#endif // VERTEXBUFFER_H
