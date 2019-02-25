#include <ctype.h>
#include "vertexAttributes.h"
#include "../Utils/log.h"

//#define DEBUG_PROJECT

VertexAttributes::VertexAttributes()
{

}

VertexAttributes::~VertexAttributes()
{

}

void VertexAttributes::addAttribute(const VertexAttribute& vAttribute)
{
    pAttributes.push_back(vAttribute);
}

bool VertexAttributes::create_interleaved_attributes(const std::vector<std::string> types, bool normalized)
{
  #ifdef DEBUG_PROJECT
    std::string vector2str = "[";
    for (auto const& s : types) { vector2str += (s + ","); }
    vector2str += "]";
    Log::getDebug().log("VertexAttributes::create_interleaved_attributes(%).", vector2str );
  #endif  


  GLsizei stride = 0; 
  const GLvoid* offset = 0; 
  GLuint location = 0;

  for (const auto& x : types)
  {
    int scalar;
    char type;
    GLenum gl_type;
    GLint size;
    try {
      char d = x[0];
      if (isdigit(d)) scalar = d; 
      else {
        Log::getError().log("VertexAttributes::create_interleaved_attributes. % '%'.", "failed to extract digit from type", x);
        return false;
      }
      type = x[1];
    } catch (const std::exception& e) { 
      Log::getError().log("VertexAttributes::create_interleaved_attributes. %", e.what());
      return false;
    }
    switch (type) {
      case 'f': size = sizeof(GL_FLOAT) * scalar; stride += size; gl_type = GL_FLOAT;  break;

      default:
        Log::getError().log("VertexAttributes::create_interleaved_attributes. Unrecognized attribute type '%'.", std::to_string(type));
        return false;
    }

    VertexAttribute va;
    va.attributeLocation = location;
    va.size = size; 
    va.type = type;
    va.normalized = normalized ? GL_FALSE : GL_TRUE; 
    va.stride = 0; // This must be updated at the end of this function. 
    va.offset = offset;
    pAttributes.push_back(va);
    location++;
  }

  // Lets update the stride.
  for (auto& x : pAttributes)
  {
    x.stride = stride;
  }
  return true;
}

void VertexAttributes::registerAttributes() const
{
    // tell to opengl how to interpret the data of the binded buffer.
    for (const auto& x : pAttributes)
    {
        glEnableVertexAttribArray(x.attributeLocation);
        glVertexAttribPointer(
            x.attributeLocation,   // attribute location. location for shader attribute.
            x.size,       // size
            x.type,       // type
            x.normalized, // normalized
            x.stride,     // stride
            x.offset      // array buffer offset.
            );
    }
}
