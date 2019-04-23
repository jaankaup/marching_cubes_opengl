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
  for (int k = -1 ; k<1 ; k++) {
  for (int j = -1 ; j<1 ; j++) {
  for (int i = -1 ; i<1 ; i++) {
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
  Log::getDebug().log("HEY!! VOXELS_PER_BLOCK = %", std::to_string((float)metadata->block_size));
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
//  glDrawArraysInstanced(GL_POINTS, 0, count, basePositions.size());
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
  for (int i=0; i<200 ; i++)
  {
    Log::getDebug().log("feedback[%] == %.", std::to_string(i), std::to_string(feedback[i]));
////////    if (feedback[i] == 0.0) break;
////////    {
////////      maskCount++;
//////////      logGLM("basePosition",basePosition);
////////      //Log::getDebug().log("feedback[%] == %.", std::to_string(i), std::to_string(feedback[i]));
////////    }
  }
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

Vertexbuffer* VertexBufferManager::optimize_vertex_buffer2(const std::string& optimized_name,
                                                           const std::string& shaderName,
                                                           const glm::vec3& basePosition,
                                                           const Vertexbuffer* vb)
{
  Log::getDebug().log("CREATING OPTIMIZED GREEN2.");

  // Instanced thing.
  
  auto metadata = ProgramState::getInstance().getMetadata();

  float X = (float)metadata->cube_count_x;
  float Y = (float)metadata->cube_count_y;
  float Z = (float)metadata->cube_count_z;

  Log::getDebug().log("CUBE_COUNTS = %,%,%", std::to_string(X),std::to_string(Y),std::to_string(Z));

  // Create cube poinst.
  int size = metadata->cube_count_x * metadata->cube_count_y * metadata->cube_count_z; 
  std::vector<float> points;
  for (int i=0 ; i<X ; ++i) {
  for (int j=0 ; j<Y ; ++j) {
  for (int k=0 ; k<Z ; ++k) {
    points.push_back((float)i);
    points.push_back((float)j);
    points.push_back((float)k);
  }}};

  std::vector<glm::vec3> basePositions;
  for (int i = -12 ; i<12 ; i++) {
  for (int j = -9 ; j<9 ; j++) {
  for (int k = -12 ; k<12 ; k++) {
    basePositions.push_back(glm::vec3(float(i)*X,float(j)*Y,float(k)*Z));
  }}};

    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * basePositions.size(), &basePositions[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*points.size(), &points[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // also set instance data
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(1, 1); // tell OpenGL this is an instanced vertex attribute.

  Log::getDebug().log("SIZE OF BASEDATA = %", std::to_string(basePositions.size()));

  Shader* shader = ShaderManager::getInstance().getShaderByName(metadata->triangulationShader);
  shader->bind();
  Texture tritable = TextureManager::getInstance().getTextureByName(metadata->tri_table_name);
  tritable.use(1);
  shader->setUniform("tri_table", 1);
  
  Texture texture = TextureManager::getInstance().getTextureByName(metadata->texture3Dname);
  texture.use(0);

  shader->setUniform("diffuse3DTexture",0);
  shader->setUniform("voxels_per_block", (float)metadata->block_size);
  Log::getDebug().log("HEY!! VOXELS_PER_BLOCK = %", std::to_string((float)metadata->block_size));
  shader->setUniform("startPoint", basePosition);
  
  // Continue instanced.
//  glEnableVertexAttribArray(1);
//  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
//  glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(float)*3 , 0);
//  glBindBuffer(GL_ARRAY_BUFFER, 0);
//  glVertexAttribDivisor(1,1);

  // Create the transform feedback buffer

//  vb->bind();
//  auto count = vb->getCount();
  //auto count = vb->getCount();
//  Log::getDebug().log("COUNT == %.", std::to_string(count));
  auto transformFeedbackCount = 200000000;
  GLuint tbo;
  glGenBuffers(1, &tbo);
//  glGenTransformFeedbacks(1, &tbo);GL_ARRAY_BUFFER
  //glBindBuffer(GL_TRANSFORM_FEEDBACK, tbo);
  glBindBuffer(GL_ARRAY_BUFFER, tbo);
//  glBindTransformFeedback(GL_TRANSFORM_FEEDBACK);
  glBufferData(GL_ARRAY_BUFFER, transformFeedbackCount /* sizeof(glm::vec3)*count*15*6 */, nullptr, /*GL_STATIC_READ*/ GL_DYNAMIC_COPY);
  //glBufferData(GL_TRANSFORM_FEEDBACK, transformFeedbackCount /* sizeof(glm::vec3)*count*15*6 */, nullptr, GL_STATIC_READ /*GL_DYNAMIC_COPY*/);

  // Perform feedback transform. Get all cube cases for the green thing.
  glEnable(GL_RASTERIZER_DISCARD);

  //glBindBufferBase(GL_ARRAY_BUFFER, 0, tbo);
  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);

  // The counter for the geomtery shader. This must be done before
  // glBeginTransformFeedback.
  GLuint query;
  glGenQueries(1,&query);
  //glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN,query);
  glBeginQuery(GL_PRIMITIVES_GENERATED, query);

  // Do the recording for cube cooridates that creates triangles.
  glBeginTransformFeedback(GL_TRIANGLES);
  //glBeginTransformFeedback(GL_POINTS);
//  glDrawArrays(GL_POINTS, 0, count);
//  glDrawArraysInstanced(GL_POINTS, 0, count, 27);
  glDrawArraysInstanced(GL_POINTS, 0, size, basePositions.size());
//  glDrawArraysInstanced(GL_POINTS, 0, count, basePositions.size());
  glEndTransformFeedback();


  glDisable(GL_RASTERIZER_DISCARD);

  glFlush();

  // Stop the counter.
  //glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
  glEndQuery(GL_PRIMITIVES_GENERATED);

  // The result of the counter.
  GLuint primitiveCount;
  glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitiveCount);

  Log::getDebug().log("PRIMITIVE_READ == %.", std::to_string(primitiveCount));

  int triangleDataCount = primitiveCount;

  unsigned int actual_data_size = primitiveCount*6*sizeof(float);
  GLfloat* feedback = new GLfloat[primitiveCount*6];
  //glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, actual_data_size, feedback);
  glGetBufferSubData(GL_ARRAY_BUFFER, 0, actual_data_size, feedback);

  glDeleteVertexArrays(1, &quadVAO);
  // Release the transfrom feedback buffer.
  glDeleteBuffers(1, &tbo);

  // Release the instanced buffers.
  glDeleteBuffers(1, &instanceVBO);

  // Release the cube points.
  glDeleteBuffers(1, &quadVBO);

//    unsigned int quadVAO, quadVBO;
  Vertexbuffer* result = createVertexBuffer(optimized_name);
  result->init();
  std::vector<std::string> types = {"3f","3f"};
  result->addData(feedback, actual_data_size, types);
  result->pDataCount = primitiveCount;
//  Log::getDebug().log("PRIMITIVECOUNT == %.", std::to_string(result->pDataCount));
//  Log::getDebug().log("SIZE_OF_FEEDBACK == %.", std::to_string(sizeof(float)*primitiveCount*6));

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
