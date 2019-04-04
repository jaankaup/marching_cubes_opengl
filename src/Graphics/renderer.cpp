#include "renderer.h"
#include "window.h"
Renderer::Renderer()
{
//  auto b = 123;
}
	
Renderer::~Renderer()
{

}

void Renderer::init()
{
    glEnable(GL_DEPTH_TEST); // Enable depth test
    glDepthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one
//    glFrontFace(GL_CW);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
//    glDisable(GL_CULL_FACE);
//    glCullFace(GL_FRONT);
    glCullFace(GL_BACK);
    glEnable(GL_PROGRAM_POINT_SIZE);
//    glEnable(GL_MULTISAMPLE);
//    glEnable(GL_TEXTURE_3D);
}

void Renderer::renderModels(const std::vector<Model>& models, const Camera& camera)
{
  glClearColor(0.0f,0.0f,0.0f,1.0f);
//  glClearColor(0.5f,0.0f,0.0f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glm::vec3 eyePosition = camera.getPosition();
  glm::mat4 viewMatrix = camera.getViewMatrix();
//  logGLM("eyePositionModels",eyePosition);
//  logGLM("viewMatrixModels",viewMatrix);

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 700.0f);
  std::vector<Command> commands;
  for (const auto& m : models)
  {
//    glm::mat4 modelMatrix = m.getModelMatrix();
    for (const auto& c : m.getCommands())
    {
      commands.push_back(c);
    }
  }

//  Log::getDebug().log("commands.size = %",std::to_string(commands.size()));
  for (const auto& com : commands)
  {
//    Log::getDebug().log("drawing = (%)",std::to_string(com.vao));
    glm::mat4 mx = com.modelMatrix;
//    logGLM("modelMatrixModels",mx);
    Shader shader = ShaderManager::getInstance().getShaderByName(com.shaderName);
    shader.bind();
    Texture texture = TextureManager::getInstance().getTextureByName(com.textureName);//{TextureType::d2,0};
    texture.use(0);
    glBindVertexArray(com.vao);
    shader.setUniform("MVP", projection * viewMatrix * mx);
    shader.setUniform("normalMatrix", glm::inverseTranspose(glm::mat3(mx)));
    shader.setUniform("M", mx);
    shader.setUniform("lights[0].color", glm::vec3(1.0f,1.0f,1.0f));
    shader.setUniform("lights[0].ambientCoeffience", 0.25f);
    shader.setUniform("lights[0].materialSpecularColor", glm::vec3(1.0f,1.0f,1.0f));
    shader.setUniform("lights[0].materialShininess", 70.0f);
    shader.setUniform("lights[0].attentuationFactor", 0.00009f);
    shader.setUniform("cameraPosition", eyePosition);
    shader.setUniform("lights[0].position", glm::vec3(12.0f,12.0f,12.0f));/* eyePosition);*/
    shader.setUniform("voxels_per_block", 6.0f);/* eyePosition);*/
    Texture tritable = TextureManager::getInstance().getTextureByName("tri_table_texture");//{TextureType::d2,0};
    switch (texture.getTextureType())
    {
      case TextureType::d2:
        shader.setUniform("diffuseTexture",0);
        break;
      case TextureType::d3:
        shader.setUniform("diffuse3DTexture",0);
        break;
    }
    tritable.use(1);
    shader.setUniform("tri_table", 1);/* eyePosition);*/

    switch (com.draw)
    {
      case GL_TRIANGLES:
        glDrawArrays(GL_TRIANGLES, com.startIndex, com.count);
        break;
      case GL_POINTS:
        glDrawArrays(GL_POINTS, com.startIndex, com.count);
        break;
    }
//    glBindVertexArray(c.vao);
//    Log::getDebug().log("drawArrasy = (%,%)",std::to_string(c.startIndex),std::to_string(c.count));
 //   Log::getDebug().log("command = (%,%)",com.shaderName,com.textureName);

  }
//  glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
  
//  shader.setUniform("MVP", projection * viewMatrix * model);
//  glm::mat3 mv = glm::mat3(viewMatrix * model);
  //shader.setUniform("normalMatrix", glm::mat3(glm::inverseTranspose(model)));
//  shader.setUniform("normalMatrix", glm::inverseTranspose(glm::mat3(model)));
//  shader.setUniform("M", model);
  
    // 0 (offset taulukossa). piirrettava kolmiomaara ts. 3*trianglecount 
//    glDrawArrays(GL_TRIANGLES, 0, 3 * triangleCount);
}

void Renderer::render(const Vertexbuffer& vb,
                      const Shader& shader,
                      int triangleCount,
                      const glm::mat4& viewMatrix,
                      const glm::vec3& eyePosition
                      )
{
  //glClearColor(0.5f,0.0f,0.0f,1.0f);
  glClearColor(0.0f,0.0f,0.0f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  Shader shader2 = ShaderManager::getInstance().getShaderByName("my3Dshader");
  shader2.bind();
  Texture texture = TextureManager::getInstance().getTextureByName("my3Dtexture");//{TextureType::d2,0};
  texture.use(0);

  vb.bind();
//  shader.bind();

  glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 700.0f);

//  logGLM("eyePositionOriginal",eyePosition);
//  logGLM("viewMatrixOriginal",viewMatrix);

  shader2.setUniform("MVP", projection * viewMatrix * model);
  shader2.setUniform("normalMatrix", glm::inverseTranspose(glm::mat3(model)));
  shader2.setUniform("M", model);
  shader2.setUniform("lights[0].color", glm::vec3(1.0f,1.0f,1.0f));
  shader2.setUniform("lights[0].ambientCoeffience", 0.25f);
  shader2.setUniform("lights[0].materialSpecularColor", glm::vec3(1.0f,1.0f,1.0f));
  shader2.setUniform("lights[0].materialShininess", 70.0f);
  shader2.setUniform("lights[0].attentuationFactor", 0.00009f);
  shader2.setUniform("cameraPosition", eyePosition);
  shader2.setUniform("lights[0].position", glm::vec3(12.0f,12.0f,12.0f));/* eyePosition);*/
  shader2.setUniform("diffuse3DTexture",0);
//  shader.setUniform("diffuseTexture", 0);

  
  //glm::mat3 mv = glm::mat3(viewMatrix * model);
  //shader.setUniform("normalMatrix", glm::mat3(glm::inverseTranspose(model)));
  
    // 0 (offset taulukossa). piirrettava kolmiomaara ts. 3*trianglecount 
    glDrawArrays(GL_TRIANGLES, 0, 3 * triangleCount);
  }
