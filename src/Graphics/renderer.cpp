#include "renderer.h"
#include "window.h"
Renderer::Renderer()
{
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
//    glEnable(GL_PROGRAM_POINT_SIZE);
//    glEnable(GL_MULTISAMPLE);
//    glEnable(GL_TEXTURE_3D);
}

void Renderer::renderModels(const std::vector<Model>& models, const Camera& camera)
{
  glClearColor(0.0f,0.0f,0.0f,1.0f);
  //glClearColor(0.2f,0.0f,0.0f,1.0f);
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
//  logGLM("eyePosition",eyePosition);
  auto startPoint = ProgramState::getInstance().getStartPoint();
  auto sumPoint = glm::vec3(std::floor(startPoint.x + 16.0f *eyePosition.x), startPoint.y, std::floor(startPoint.z + 16.0f * eyePosition.z));
//  logGLM("sumPoint",sumPoint);

//  logGLM("startPopint",startPoint);
//  Log::getDebug().log("startPoint = %",std::to_string(startPoint_Y));
    //auto sp = glm::vec3(eyePosition.x, startPoint_Y, eyePosition.z); // - glm::vec3(0.0f,eyePosition.y,0.0f);
    //logGLM("sp",sp);
    //ProgramState::getInstance().setStartPoint(sp);

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
    shader.setUniform("lights[0].position", glm::vec3(8.0f,8.0f,8.0f));/* eyePosition);*/
    shader.setUniform("voxels_per_block",  ProgramState::getInstance().getVoxelsPerBlock());
    shader.setUniform("startPoint", sumPoint /* sumPoint */); //  ProgramState::getInstance().getStartPoint());
    shader.setUniform("cubeMask",  ProgramState::getInstance().getCubeMaskCeil());
    shader.setUniform("debugMask", ProgramState::getInstance().getDebugCube() ? 1.0f : 0.0f);

    Texture tritable = TextureManager::getInstance().getTextureByName("tri_table_texture");//{TextureType::d2,0};

    if (com.shaderName == "marchingShaderLine" && !ProgramState::getInstance().getWireframe()) continue; 
//    Log::getInfo().log(shaderN);
//    glm::mat4 mx = com.modelMatrix;
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
  }
}

