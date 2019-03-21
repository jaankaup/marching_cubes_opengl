#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "vertexbuffer.h"

class Model
{
	public:
		Model();
		~Model();

	private:
    GLuint pVao;
    Vertexbuffer pVertexBuffer;
};


#endif // MODEL_H
