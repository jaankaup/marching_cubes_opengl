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
//    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
//    glCullFace(GL_BACK);
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
  float yhyy = sin((float)lkm*0.008);
  float yhyy2 = sin((float)lkm*0.00008);
//  Log::getDebug().log("yhyy: %", std::to_string(yhyy));
  vb.bind();
  shader.bind();
  //vb.bind();
  glm::vec3 lightPosition = glm::vec3(10.0f,-10.0f,-10.0f);
  glm::vec3 eyePosition = glm::vec3(5.0f,25.0f,25.0f);

//  shader.setUniform("lightPosition", lightPosition);
  shader.setUniform("cameraPosition", eyePosition);
  shader.setUniform("lights[0].position", lightPosition);
  shader.setUniform("lights[0].color", glm::vec3(1.0f,1.0f,1.0f));
  shader.setUniform("lights[0].ambientCoeffience", 0.25f);
  shader.setUniform("lights[0].materialSpecularColor", glm::vec3(1.0f,1.0f,1.0f));
  shader.setUniform("lights[0].materialShininess", 70.0f);
  shader.setUniform("lights[0].attentuationFactor", 0.000009f);
  //shader.setUniform("lights[0].attentuationFactor", 0.000009f);

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  glm::mat4 view = glm::lookAt(eyePosition, glm::vec3(0.0,0.0,0.0), glm::vec3(0.0f, 1.0f,  0.0f));
  //glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f)); // glm::mat4(yhyy*0.2f);
  glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
  glm::mat4 rotation = glm::rotate(yhyy2*180.0f, glm::vec3(1.0f,1.0f,1.0f));
  glm::mat4 model = glm::translate(rotation * scale, glm::vec3((-1.0f)*yhyy*5.0f,yhyy*5.0f, yhyy*1.0f)); // glm::mat4(yhyy*0.2f);
  shader.setUniform("normalMatrix", glm::inverseTranspose(glm::mat3(model)));

  shader.setUniform("MVP", projection * view * model);
  shader.setUniform("M", model);
//  shader.setUniform("V", view);
//  shader.setUniform("P", projection);

  glDrawArrays(GL_TRIANGLES, 0, 36);
}
