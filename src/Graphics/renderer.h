#ifndef RENDERER_H
#define RENDERER_H
#include <iostream>
#include <vector>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "shader.h"
#include "vertexbuffer.h"
#include "../Utils/log.h"

//#include "misc.h"

/** A simple shader class with basic shader operations. */
class Renderer
{
	public:
		Renderer();
		~Renderer();

    void init();
    void render(const Vertexbuffer& vb, const Shader& shader);

	private:

    int lkm = 0;
    bool pah = true;
};


#endif // RENDERER_H
