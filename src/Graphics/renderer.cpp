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
    glFrontFace(GL_CW);
//    //glFrontFace(GL_CCW);
    glCullFace(GL_FRONT);
//    //glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    //glEnable(GL_MULTISAMPLE);
}

void Renderer::render(const Vertexbuffer& vb,
                      const Shader& shader
//                      const glm::vec3& lightPosition,
//                      const glm::vec3& eyePosition,
//                      const glm::mat4& mvp,
//                      const Window& window
                      )
{
  //Log::getDebug().log("rendering: %", std::to_string(lkm));
  lkm++;
  glClearColor(0.0f,0.0f,0.0f,1.0f);
//  if (pah) glClearColor(0.8f,0.1f,0.3f,1.0f);
//  else glClearColor(0.0f,0.0f,0.0f,1.0f);
  pah = !pah;
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  auto sdf = 1;

//  if (pah){ glFrontFace(GL_CW); glCullFace(GL_FRONT); }
//  else { glFrontFace(GL_CCW); glCullFace(GL_BACK); }
//  auto h = vb.getHandle();
  float yhyy = sin((float)lkm*0.02)*5.0;
//  Log::getDebug().log("yhyy: %", std::to_string(yhyy));
  vb.bind();
  shader.bind();
  //vb.bind();
  glm::vec3 lightPosition = glm::vec3(15.0f,25.0f,5.0f);
  glm::vec3 eyePosition = glm::vec3(1.0f,1.0f,yhyy*1.0f);

//  shader.setUniform("LightPosition_worldspace", glm::vec3(10.0f,50.0f,10.0f));
//  shader.setUniform("lightPosition", eyePosition);
//  shader.setUniform("lights[0].position", lightPosition);
//  shader.setUniform("lights[0].color", glm::vec3(1.0f,1.0f,1.0f));
//  shader.setUniform("lights[0].ambientCoeffience", 0.05f);
//  shader.setUniform("lights[0].materialSpecularColor", glm::vec3(1.0f,1.0f,1.0f));
//  shader.setUniform("lights[0].materialShininess", 70.0f);
//  shader.setUniform("lights[0].attentuationFactor", 0.000009f);

  glm::mat4 projection = glm::perspective(glm::radians(150.0f), 16.0f / 9.0f, 0.1f, 1000.f);
  glm::mat4 view = glm::lookAt(eyePosition, glm::vec3(0.0,0.0,0.0), glm::vec3(0.0f, 1.0f,  0.0f));
  glm::mat4 model = glm::mat4(1.0f);// glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(yhyy*0.2f)), glm::vec3(yhyy*0.2f, 1.0f, 1.0f)); // glm::mat4(yhyy*0.2f);
//  shader.setUniform("normalMatrix", glm::inverseTranspose(glm::mat3(model)));

  shader.setUniform("MVP", projection * view * model);
//  shader.setUniform("M", model);
//  shader.setUniform("V", view);
//  shader.setUniform("P", projection);

  glDrawArrays(GL_TRIANGLES, 0, 12);
}
