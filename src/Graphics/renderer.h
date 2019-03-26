#ifndef RENDERER_H
#define RENDERER_H
#include <iostream>
#include <vector>
#include <array>
#include <GL/glew.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "shader.h"
#include "vertexbuffer.h"
#include "texture.h"
#include "../Utils/log.h"

//#include "misc.h"

/** A simple shader class with basic shader operations. */
class Renderer
{
	public:
		Renderer();
		~Renderer();

    void init();
    void render(const Vertexbuffer& vb, const Shader& shader, int triangleCount, const glm::mat4& viewMatrix,const glm::vec3& eyePosition);

	private:

//    int lkm = 0;
//    bool pah = true;
};


#endif // RENDERER_H
