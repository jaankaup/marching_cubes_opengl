#ifndef VERTEXATTRIBUES_H
#define VERTEXATTRIBUES_H

#include <vector>
#include <array>
#include <GL/glew.h>

struct VertexAttribute
{
  GLuint attributeLocation;
  GLint size;
  GLenum type;
  GLboolean normalized;
  GLsizei stride;
  const GLvoid* offset;
};

/** A class for gl vertexattributes. */
class VertexAttributes
{
	public:
		VertexAttributes();
		~VertexAttributes();

    void addAttribute(const VertexAttribute& vAttribute);

    bool create_interleaved_attributes(const std::vector<std::string> types, bool normalized); 

	private:
    std::vector<VertexAttribute> pAttributes;
};


#endif // VERTEXATTRIBUES_H
