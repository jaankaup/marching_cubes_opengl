#ifndef VERTEXBUFFERMANAGER_H
#define VERTEXBUFFERMANAGER_H

#include <vector>
#include <tuple>
#include <stdexcept>
#include "vertexbuffer.h"
#include "../Utils/log.h"

class Vertexbuffer;


class VertexBufferManager
{
  public:
    /// Creates and return a reference to the VertexBufferManager object.
    static VertexBufferManager& getInstance();
  
    Vertexbuffer createVertexBuffer(const std::string& name);
    Vertexbuffer getVertexBufferByName(const std::string& name) const;
  
  private:
    VertexBufferManager();
    ~VertexBufferManager();
  
    /// Name:VertexBuffer pairs.
    std::vector<std::tuple<const std::string, Vertexbuffer>> pVertexBuffers;
};

#endif // VERTEXBUFFERMANAGER_H
