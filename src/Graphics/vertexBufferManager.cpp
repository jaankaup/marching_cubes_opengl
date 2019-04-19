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
                                                          const Vertexbuffer* vb)
{
  Log::getDebug().log("CREATING OPTIMIZED GREEN.");

  // Instanced thing.
  
  auto metadata = ProgramState::getInstance().getMetadata();

//    int cube_count_x = 0;
//    int cube_count_y = 0;
//    int cube_count_z = 0;
  float X = (float)metadata->cube_count_x;
  float Y = (float)metadata->cube_count_y;
  float Z = (float)metadata->cube_count_z;

  Log::getDebug().log("CUBE_COUNTS = %,%,%", std::to_string(X),std::to_string(Y),std::to_string(Z));

 // glm::vec3 basePositions[27]; 
  std::vector<glm::vec3> basePositions;
  for (int k = -6 ; k<4 ; k++) {
  for (int j = -6 ; j<4 ; j++) {
  for (int i = -6 ; i<4 ; i++) {
    basePositions.push_back(glm::vec3(float(i)*X,float(j)*Y,float(k)*Z));
//    Log::getDebug().log("(i,j,k) = %,%,%", std::to_string(i),std::to_string(j),std::to_string(k));
//    logGLM("basePosition",glm::vec3(i*X,j*Y,k*Z));
  }}};
//  for (const auto& q : basePositions)
//  {
//    logGLM("basePosition",q);
//  }
  Log::getDebug().log("SIZE OF BASEDATA = %", std::to_string(basePositions.size()));

//  glm::vec3 basePositions[27] = {glm::vec3(0.0f,0.0f,0.0f),glm::vec3(X,0.0f,0.0f),glm::vec3(-X,0.0f,0.0f), 
//                                glm::vec3(0.0f,Y,0.0f)   ,glm::vec3(0.0f,-Y,0.0f),
//                                glm::vec3(0.0f,0.0f,Z),glm::vec3(0.0f,0.0f,-Z),
//                                glm::vec3(X,Y,0.0f),glm::vec3(X,-Y,0.0f),
//                                glm::vec3(X,0.0f,Z),glm::vec3(X,0.0f,-Z),
//                                glm::vec3(X,Y,Z),glm::vec3(X,Y,-Z),
//                                glm::vec3(X,-Y,Z),glm::vec3(X,-Y,-Z),
//                                glm::vec3(-X,Y,0.0f),glm::vec3(-X,-Y,0.0f),
//                                glm::vec3(-X,0.0f,Z),glm::vec3(-X,0.0f,-Z),
//                                glm::vec3(-X,Y,Z),glm::vec3(-X,Y,-Z),
//                                glm::vec3(-X,-Y,Z),glm::vec3(-X,-Y,-Z),
//                                glm::vec3(0.0f,Y,Z),glm::vec3(0.0f,Y,-Z)
//                                };
  GLuint instanceVBO;
  glGenBuffers(1, &instanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*(basePositions.size()), &basePositions[0], GL_STATIC_DRAW);
//  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*27, &basePositions, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);


  Shader* shader = ShaderManager::getInstance().getShaderByName(metadata->triangulationShader);
  shader->bind();
  Texture tritable = TextureManager::getInstance().getTextureByName(metadata->tri_table_name);
  tritable.use(1);
  shader->setUniform("tri_table", 1);
  
  Texture texture = TextureManager::getInstance().getTextureByName(metadata->texture3Dname);
  texture.use(0);

  shader->setUniform("diffuse3DTexture",0);
  shader->setUniform("voxels_per_block", (float)metadata->block_size);
  shader->setUniform("startPoint", basePosition);
  
  vb->bind();

  // Continue instanced.
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(float)*3 , 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glVertexAttribDivisor(1,1);

  // Create the transform feedback buffer

//  vb->bind();
  auto count = vb->getCount();
  //auto count = vb->getCount();
  Log::getDebug().log("COUNT == %.", std::to_string(count));
  auto transformFeedbackCount = 200000000;
  GLuint tbo;
  glGenBuffers(1, &tbo);
  glBindBuffer(GL_ARRAY_BUFFER, tbo);
  glBufferData(GL_ARRAY_BUFFER, transformFeedbackCount /* sizeof(glm::vec3)*count*15*6 */, nullptr, GL_STATIC_READ);

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
//  glDrawArrays(GL_POINTS, 0, count);
//  glDrawArraysInstanced(GL_POINTS, 0, count, 27);
  glDrawArraysInstanced(GL_POINTS, 0, count, basePositions.size());
  glEndTransformFeedback();


  glDisable(GL_RASTERIZER_DISCARD);

  glFlush();

  // Stop the counter.
  glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

  // The result of the counter.
  GLuint primitiveCount;
  glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitiveCount);

  Log::getDebug().log("PRIMITIVE_READ == %.", std::to_string(primitiveCount));

  int triangleDataCount = primitiveCount;

  unsigned int actual_data_size = primitiveCount*6*sizeof(float);
  GLfloat* feedback = new GLfloat[primitiveCount*6];
  glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, actual_data_size, feedback);

  // Release the transfrom feedback buffer.
  glDeleteBuffers(1, &tbo);

  // Release the instanced buffers.
  glDeleteBuffers(1, &instanceVBO);

//  Log::getDebug().log("sizeof(feedback) == %.", std::to_string(sizeof(feedback)));
//  int maskCount = 0;
//  Log::getDebug().log("feedback[0] == %.", std::to_string(feedback[0]));
//////  for (int i=0; i<200 ; i++)
//////  {
//////    Log::getDebug().log("feedback[%] == %.", std::to_string(i), std::to_string(feedback[i]));
////////    if (feedback[i] == 0.0) break;
////////    {
////////      maskCount++;
//////////      logGLM("basePosition",basePosition);
////////      //Log::getDebug().log("feedback[%] == %.", std::to_string(i), std::to_string(feedback[i]));
////////    }
//////  }
//  Log::getDebug().log("maskCount = %", std::to_string(maskCount));
  Vertexbuffer* result = createVertexBuffer(optimized_name);
  result->init();
  std::vector<std::string> types = {"3f","3f"};
  result->addData(feedback, actual_data_size, types);
  result->pDataCount = primitiveCount;
  Log::getDebug().log("PRIMITIVECOUNT == %.", std::to_string(result->pDataCount));
  Log::getDebug().log("SIZE_OF_FEEDBACK == %.", std::to_string(sizeof(float)*primitiveCount*6));

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
