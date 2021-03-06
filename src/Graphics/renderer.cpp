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
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_MULTISAMPLE);
}

void Renderer::renderModels(const Camera& camera)
{
  glClearColor(0.0f,0.0f,0.0f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glm::vec3 eyePosition = camera.getPosition();
  glm::mat4 viewMatrix = camera.getMatrix();

  auto models = ModelManager::getInstance().getModels();
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.001f, 100.0f);
  std::vector<Command> commands;
  for (const auto m : models)
  {
    if (m->getDisabled()) continue;
    for (const auto& c : *m->getCommands())
    {
      commands.push_back(c);
    }
  }

    float voxels_per_block = ProgramState::getInstance().getVoxelsPerBlock();
  for (const auto& com : commands)
  {
    Shader* shader = ShaderManager::getInstance().getShaderByName(com.shaderName);
    shader->bind();
    shader->setUniform("voxels_per_block", voxels_per_block);

    auto metadata = ProgramState::getInstance().getMetadata();
    auto tName = metadata->texture3Dname;
    glm::mat4 mx = com.modelMatrix ;
    Texture texture = TextureManager::getInstance().getTextureByName(tName);//{TextureType::d2,0};
    texture.use(0);
    glBindVertexArray(com.vao);

    // TODO: add lights only once.
    shader->setUniform("MVP", projection * viewMatrix * mx);
    shader->setUniform("normalMatrix", glm::inverseTranspose(glm::mat3(mx)));
    shader->setUniform("M", mx);
    shader->setUniform("lights[0].color", glm::vec3(1.0f,1.0f,1.0f));
    shader->setUniform("lights[0].ambientCoeffience", 0.25f);
    shader->setUniform("lights[0].materialSpecularColor", glm::vec3(1.0f,1.0f,1.0f));
    shader->setUniform("lights[0].materialShininess", 70.0f);
    shader->setUniform("lights[0].attentuationFactor", 0.00009f);
    shader->setUniform("cameraPosition", eyePosition);
    shader->setUniform("lights[0].position", glm::vec3(8.0f,8.0f,8.0f));/* eyePosition);*/

    Texture tritable = TextureManager::getInstance().getTextureByName("tri_table_texture");//{TextureType::d2,0};

    // TODO: wireframe is not available in this version. Remove these lines.
    if (com.shaderName == "marchingShaderLine" && !ProgramState::getInstance().getWireframe()) continue; 
    switch (texture.getTextureType())
    {
      case TextureType::d1:
        // Do nothing.
        break;
      case TextureType::d2:
        shader->setUniform("diffuseTexture",0);
        break;
      case TextureType::d3:
        shader->setUniform("diffuse3DTexture",0);
        break;
    }
    tritable.use(1);
    shader->setUniform("tri_table", 1);

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

