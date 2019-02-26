#include "vertexbuffer.h"
#include "../Utils/log.h"

Vertexbuffer::Vertexbuffer()
{
  auto joo = 13;
}

Vertexbuffer::~Vertexbuffer()
{
  if (pId != 0) glDeleteBuffers(1,&pId);
  glDeleteVertexArrays(1, &pVAO);
}

void Vertexbuffer::bind() const
{
//  Log::getDebug().log("Binding vao %.",std::to_string(pVAO));
  glBindVertexArray(pVAO);
}

void Vertexbuffer::init()
{
    glGenVertexArrays(1, &pVAO);
    glGenBuffers(1,&pId);
    Log::getDebug().log("Created a vertex buffer: %", std::to_string(pId));
    pTarget = GL_ARRAY_BUFFER;
    pUsage = GL_STATIC_DRAW;
}

GLuint Vertexbuffer::getHandle() const
{
  return pId;
}

void Vertexbuffer::addData(const void* data, unsigned int size) const
{
  Log::getDebug().log("Vertexbuffer::addData: %", std::to_string(size));
  bind();
  glBindBuffer(pTarget, pId);
  glBufferData(pTarget,size, data, pUsage);
  VertexAttributes vas;
  std::vector<std::string> types = {"3f","3f"};
  vas.create_interleaved_attributes(types, false);
  vas.registerAttributes();
  glBindVertexArray(0);
}

void Vertexbuffer::createExampleCube()
{

  std::vector<glm::vec3> vData = {glm::vec3(1.0f , -1.0f, -1.0f),
                                  glm::vec3(1.0f , -1.0f, 1.0f),
                                  glm::vec3(-1.0f, -1.0f, 1.0f),
                                  glm::vec3(-1.0f, -1.0f, -1.0f),
                                  glm::vec3(1.0f , 1.0f, -1.0f),
                                  glm::vec3(1.0f, 1.0f, 1.0f),
                                  glm::vec3(-1.0f, 1.0f, 1.0f),
                                  glm::vec3(-1.0f, 1.0f, -1.0f)};

  std::vector<glm::vec3> nData = {glm::vec3(0.0f , 0.0f , -1.0f),
                                  glm::vec3(-1.0f, -0.0f, 0.0f),
                                  glm::vec3(0.0f, -0.0f, 1.0f),
                                  glm::vec3(0.0f, 0.0f , 1.0f),
                                  glm::vec3(1.0f , -0.0f, 0.0f),
                                  glm::vec3(1.0f , 0.0f , 0.0f),
                                  glm::vec3(0.0f , 1.0f , 0.0f),
                                  glm::vec3(0.0f, -1.0f, 0.0f)};
//  std::vector<glm::vec3> vData = {glm::vec3(0.0f, 0.0f, 0.0f),
//                                  glm::vec3(0.0f, 0.0f, 1.0f),
//                                  glm::vec3(0.0f, 1.0f, 0.0f),
//                                  glm::vec3(0.0f, 1.0f, 1.0f),
//                                  glm::vec3(1.0f, 0.0f, 0.0f),
//                                  glm::vec3(1.0f, 0.0f, 1.0f),
//                                  glm::vec3(1.0f, 1.0f, 0.0f),
//                                  glm::vec3(1.0f, 1.0f, 1.0f)};

//  std::vector<glm::vec3> nData = {glm::vec3(0.0, 0.0, 0.0),
//                                  glm::vec3(0.0, 0.0,-1.0),
//                                  glm::vec3(0.0, 1.0, 0.0),
//                                  glm::vec3(0.0,-1.0, 0.0),
//                                  glm::vec3(1.0, 0.0, 0.0),
//                                  glm::vec3(-1.0, 0.0, 0.0)};
  // Face 1
//  f 5/1/1 1/2/1 4/3/1
  pData.push_back(vData[4]);
  pData.push_back(nData[0]);
  pData.push_back(vData[0]);
  pData.push_back(nData[0]);
  pData.push_back(vData[3]);
  pData.push_back(nData[0]);

  // Face2
//  f 5/1/1 4/3/1 8/4/1
  pData.push_back(vData[4]);
  pData.push_back(nData[0]);
  pData.push_back(vData[3]);
  pData.push_back(nData[0]);
  pData.push_back(vData[7]);
  pData.push_back(nData[0]);

  // Face3
//  f 3/5/2 7/6/2 8/7/2
  pData.push_back(vData[2]);
  pData.push_back(nData[1]);
  pData.push_back(vData[6]);
  pData.push_back(nData[1]);
  pData.push_back(vData[7]);
  pData.push_back(nData[1]);

  // Face4
//  f 3/5/2 8/7/2 4/8/2
  pData.push_back(vData[2]);
  pData.push_back(nData[1]);
  pData.push_back(vData[7]);
  pData.push_back(nData[1]);
  pData.push_back(vData[3]);
  pData.push_back(nData[1]);

  // Face5
//  f 2/9/3 6/10/3 3/5/3
  pData.push_back(vData[1]);
  pData.push_back(nData[2]);
  pData.push_back(vData[5]);
  pData.push_back(nData[2]);
  pData.push_back(vData[2]);
  pData.push_back(nData[2]);

  // Face6
//  f 6/10/4 7/6/4 3/5/4
  pData.push_back(vData[5]);
  pData.push_back(nData[3]);
  pData.push_back(vData[6]);
  pData.push_back(nData[3]);
  pData.push_back(vData[2]);
  pData.push_back(nData[3]);

  // Face7
//  f 1/2/5 5/1/5 2/9/5
  pData.push_back(vData[0]);
  pData.push_back(nData[4]);
  pData.push_back(vData[4]);
  pData.push_back(nData[4]);
  pData.push_back(vData[1]);
  pData.push_back(nData[4]);

  // Face8
//  f 5/1/6 6/10/6 2/9/6
  pData.push_back(vData[4]);
  pData.push_back(nData[5]);
  pData.push_back(vData[5]);
  pData.push_back(nData[5]);
  pData.push_back(vData[1]);
  pData.push_back(nData[5]);

  // Face9
//  f 5/1/7 8/11/7 6/10/7
  pData.push_back(vData[4]);
  pData.push_back(nData[6]);
  pData.push_back(vData[7]);
  pData.push_back(nData[6]);
  pData.push_back(vData[5]);
  pData.push_back(nData[6]);

  // Face10
 // f 8/11/7 7/12/7 6/10/7
  pData.push_back(vData[7]);
  pData.push_back(nData[6]);
  pData.push_back(vData[6]);
  pData.push_back(nData[6]);
  pData.push_back(vData[5]);
  pData.push_back(nData[6]);

  // Face11
//  f 1/2/8 2/9/8 3/13/8
  pData.push_back(vData[0]);
  pData.push_back(nData[7]);
  pData.push_back(vData[1]);
  pData.push_back(nData[7]);
  pData.push_back(vData[2]);
  pData.push_back(nData[7]);

  // Face12
//  f 1/2/8 3/13/8 4/14/8
  pData.push_back(vData[0]);
  pData.push_back(nData[7]);
  pData.push_back(vData[2]);
  pData.push_back(nData[7]);
  pData.push_back(vData[3]);
  pData.push_back(nData[7]);
//  f  1//2  7//2  5//2
//f  1//2  3//2  7//2
//f  1//6  4//6  3//6
//f  1//6  2//6  4//6 
//f  3//3  8//3  7//3
//f  3//3  4//3  8//3  
//f  5//5  7//5  8//5 
//f  5//5  8//5  6//5 
//f  1//4  5//4  6//4 
//f  1//4  6//4  2//4 
//f  2//1  6//1  8//1 
//f  2//1  8//1  4//1
//
  addData(&pData[0], pData.size() * sizeof(glm::vec3));
}
