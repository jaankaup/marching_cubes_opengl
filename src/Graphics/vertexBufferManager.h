#ifndef VERTEXBUFFERMANAGER_H
#define VERTEXBUFFERMANAGER_H

#include <vector>
#include <tuple>
#include <stdexcept>
#include "vertexbuffer.h"
#include "texture.h"
#include "textureManager.h"
#include "shader.h"
#include "shaderManager.h"
#include "../Utils/log.h"
#include "../Utils/misc.h"

class Vertexbuffer;


class VertexBufferManager
{
  public:
    /// Creates and return a reference to the VertexBufferManager object.
    static VertexBufferManager& getInstance();
  
    Vertexbuffer* createVertexBuffer(const std::string& name);
    Vertexbuffer* getVertexBufferByName(const std::string& name);

    Vertexbuffer* optimize_vertex_buffer(const std::string& optimized_name,
                                         const std::string& shaderName,
                                         const glm::vec3& basePosition,
                                         const std::string& texture3D,
                                         float voxels_per_block,
//                                         const glm::vec3& mvp, // Do we actually need this?
                                         const Vertexbuffer* vb);
  
  private:
    VertexBufferManager();
    ~VertexBufferManager();
  
    std::vector<std::tuple<const std::string, Vertexbuffer>> pVertexBuffers;

};

#endif // VERTEXBUFFERMANAGER_H
