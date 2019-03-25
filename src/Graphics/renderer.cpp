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
//    glEnable(GL_MULTISAMPLE);
//    glEnable(GL_TEXTURE_3D);
}

void Renderer::render(const Vertexbuffer& vb,
                      const Shader& shader,
                      int triangleCount,
                      const glm::mat4& viewMatrix,
                      const glm::vec3& eyePosition
                      )
{
  glClearColor(0.5f,0.0f,0.0f,1.0f);
  //glClearColor(0.0f,0.0f,0.0f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  vb.bind();
//  shader.bind();

  shader.setUniform("lights[0].color", glm::vec3(1.0f,1.0f,1.0f));
  shader.setUniform("lights[0].ambientCoeffience", 0.25f);
  shader.setUniform("lights[0].materialSpecularColor", glm::vec3(1.0f,1.0f,1.0f));
  shader.setUniform("lights[0].materialShininess", 70.0f);
  shader.setUniform("lights[0].attentuationFactor", 0.00009f);
//  shader.setUniform("diffuseTexture", 0);

  shader.setUniform("cameraPosition", eyePosition);
  shader.setUniform("lights[0].position", glm::vec3(12.0f,12.0f,12.0f));/* eyePosition);*/
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 700.0f);
  glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
  
  shader.setUniform("MVP", projection * viewMatrix * model);
  glm::mat3 mv = glm::mat3(viewMatrix * model);
  //shader.setUniform("normalMatrix", glm::mat3(glm::inverseTranspose(model)));
  shader.setUniform("normalMatrix", glm::inverseTranspose(glm::mat3(model)));
  shader.setUniform("M", model);
  
    // 0 (offset taulukossa). piirrettava kolmiomaara ts. 3*trianglecount 
    glDrawArrays(GL_TRIANGLES, 0, 3 * triangleCount);
  }
