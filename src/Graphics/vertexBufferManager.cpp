#include "vertexBufferManager.h"

VertexBufferManager::VertexBufferManager()
{

}

VertexBufferManager::~VertexBufferManager()
{
   for (const auto& data : pVertexBuffers)
   {
     std::get<1>(data).dispose();
   }
}

Vertexbuffer* VertexBufferManager::createVertexBuffer(const std::string& name)
{
  Vertexbuffer s;
  s.init();
  auto vertexBufferData = std::make_tuple(name,s);
  pVertexBuffers.push_back(vertexBufferData);
  return getVertexBufferByName(name);
}

VertexBufferManager& VertexBufferManager::getInstance()
{
    static VertexBufferManager instance;
    return instance;
}

Vertexbuffer* VertexBufferManager::getVertexBufferByName(const std::string& name)
{
    for (const auto& obj : pVertexBuffers)
    {
        if (std::get<0>(obj) == name)
        {
            auto result = &(std::get<1>(obj));
            return const_cast<Vertexbuffer*>(result);
        }
    }
    throw std::runtime_error("VertexBufferManager::getVertexBufferByName(" + name + "). No such vertexBuffer.");
}

Vertexbuffer* VertexBufferManager::optimize_vertex_buffer(const std::string& optimized_name,
                                                          const std::string& shaderName,
                                                          const glm::vec3& basePosition,
                                                          const std::string& texture3D,
                                                          float voxels_per_block,
//                                                          const glm::vec3& mvp,
                                                          const Vertexbuffer* vb)
{
  Log::getDebug().log("CREATING OPTIMIZED GREEN.");
  Log::getDebug().log("optimized_name == %.", optimized_name);
  Log::getDebug().log("shaderName == %.", shaderName);
  logGLM("basePosition",basePosition);
  Log::getDebug().log("texture3D == %.", texture3D);
  Log::getDebug().log("voxels_per_block == %.", std::to_string(voxels_per_block));
  Shader* shader = ShaderManager::getInstance().getShaderByName(shaderName);
  shader->bind();
  Texture tritable = TextureManager::getInstance().getTextureByName("tri_table_texture");
  tritable.use(1);
  shader->setUniform("tri_table", 1);/* eyePosition);*/
  
  Texture texture = TextureManager::getInstance().getTextureByName(texture3D);
  texture.use(0);

  shader->setUniform("diffuse3DTexture",0);
//  shader->setUniform("MVP", mvp);
  shader->setUniform("voxels_per_block", voxels_per_block);
  shader->setUniform("startPoint", basePosition);
  
  vb->bind();

  // Create transform feedback buffer

  auto count = vb->getCount();
//  Log::getDebug().log("count == %.", std::to_string(count));
  GLuint tbo;
  glGenBuffers(1, &tbo);
  glBindBuffer(GL_ARRAY_BUFFER, tbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*count*3, nullptr, GL_STATIC_READ);

  // Perform feedback transform. Get all cube cases for the green thing.
  glEnable(GL_RASTERIZER_DISCARD);

  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);

  // The counter for the geomtery shader. This must be done before
  // glBeginTransformFeedback.
  GLuint query;
  glGenQueries(1,&query);
  glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN,query);

  // Do the recording for cube cooridates that creates triangles.
  glBeginTransformFeedback(GL_TRIANGLES);
  glDrawArrays(GL_POINTS, 0, count);
  glEndTransformFeedback();


  glDisable(GL_RASTERIZER_DISCARD);

  glFlush();

  // Stop the counter.
  glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

  // The result of the counter.
  GLuint primitiveCount;
  glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitiveCount);

  Log::getDebug().log("primitiveCounter == %.", std::to_string(primitiveCount));

  // Fetch the non empty cube coordinates.
  GLfloat* feedback = new GLfloat[primitiveCount*3];
  glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(float)*primitiveCount*3, feedback);

  // Release the transfrom feedback buffer.
  glDeleteBuffers(1, &tbo);

//  Log::getDebug().log("sizeof(feedback) == %.", std::to_string(sizeof(feedback)));
//  int maskCount = 0;
//  Log::getDebug().log("feedback[0] == %.", std::to_string(feedback[0]));
//  for (int i=0; i<primitiveCount ; i++)
//  {
//    Log::getDebug().log("feedback[%] == %.", std::to_string(i), std::to_string(feedback[i]));
////    if (feedback[i] == 0.0) break;
////    {
////      maskCount++;
//////      logGLM("basePosition",basePosition);
////      //Log::getDebug().log("feedback[%] == %.", std::to_string(i), std::to_string(feedback[i]));
////    }
//  }
//  Log::getDebug().log("maskCount = %", std::to_string(maskCount));
  Vertexbuffer* result = createVertexBuffer(optimized_name);
  result->init();
  std::vector<std::string> types = {"3f"};
  result->addData(feedback, sizeof(float)*primitiveCount*3, types);
  result->pDataCount = primitiveCount;

  delete[] feedback;
//  result-> init_transform_feedback(tbo, primitiveCount/3);

  return result;
}

bool VertexBufferManager::deleteVertexBuffer(const std::string& name)
{
   for (unsigned int i=0 ; i < pVertexBuffers.size() ; i++)
   {
     if (std::get<0>(pVertexBuffers[i]) == name)
     {
       auto haaaaa = pVertexBuffers.begin() + 1;
       std::get<1>(pVertexBuffers[i]).dispose();
       pVertexBuffers.erase(haaaaa);
       return true;
     }
   }
   return false;
}
