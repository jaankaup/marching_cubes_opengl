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
    //glEnable(GL_MULTISAMPLE);
}

void Renderer::render(const Vertexbuffer& vb,
                      const Shader& shader,
                      int triangleCount
//                      const glm::vec3& lightPosition,
//                      const glm::vec3& eyePosition,
//                      const glm::mat4& mvp,
//                      const Window& window
                      )
{
  bool normaalitMukana = true;
  //Log::getDebug().log("rendering: %", std::to_string(lkm));
  lkm++;
  glClearColor(0.0f,0.0f,0.0f,1.0f);
//  if (pah) glClearColor(0.8f,0.1f,0.3f,1.0f);
//  else glClearColor(0.0f,0.0f,0.0f,1.0f);
  pah = !pah;
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

//  if (pah){ glFrontFace(GL_CW); glCullFace(GL_FRONT); }
//  else { glFrontFace(GL_CCW); glCullFace(GL_BACK); }
//  auto h = vb.getHandle();
  float yhyy = sin((float)lkm*0.008f);
  float yhyy2 = sin((float)lkm*0.00008f);
  float yhyy3 = cos((float)lkm*0.00005f);
//  Log::getDebug().log("yhyy: %", std::to_string(yhyy));
  vb.bind();
  shader.bind();
  //vb.bind();
  glm::vec3 lightPosition = glm::vec3(10.0f,-10.0f,-10.0f);
  glm::vec3 eyePosition = glm::vec3(5.0f,5.0f,5.0f);
//
//  shader.setUniform("lightPosition", lightPosition);
  if (normaalitMukana)
  {
  shader.setUniform("cameraPosition", eyePosition);
  shader.setUniform("lights[0].position", lightPosition);
  shader.setUniform("lights[0].color", glm::vec3(1.0f,1.0f,1.0f));
  shader.setUniform("lights[0].ambientCoeffience", 0.25f);
  shader.setUniform("lights[0].materialSpecularColor", glm::vec3(1.0f,1.0f,1.0f));
  shader.setUniform("lights[0].materialShininess", 70.0f);
  shader.setUniform("lights[0].attentuationFactor", 0.000009f);
  //shader.setUniform("lights[0].attentuationFactor", 0.000009f);
  }

  for (int i=0; i<1; i++)
  {

    float a = 0.0f;
    float b = 0.0f;
    float c = 0.0f;
    float d = 0.0f;
    if (i % 2 == 0)
    {
       a = yhyy2;
       b = (i*1.5f)*yhyy3;
       c = yhyy*(2.0f*i);
       d = yhyy3*(1.0f*i);

    }

    else
    {
       a = (-1.0f)*yhyy2+(i*1.0f);
       b = (-1.0f*i)*yhyy3;
       c = yhyy + (i * 2.0f);
       d = yhyy2 + yhyy3 + (1.0f * i);

    }
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(eyePosition, glm::vec3(0.0,0.0,0.0), glm::vec3(0.0f, 1.0f,  0.0f));
    //glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f)); // glm::mat4(yhyy*0.2f);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
    glm::mat4 rotation = glm::rotate(a*180.0f, glm::vec3(1.0f,b*1.0f,c*1.0f));
    glm::mat4 model = glm::translate(rotation * scale, glm::vec3(b*2.0f,c*2.0f, d*2.0f)); // glm::mat4(yhyy*0.2f);
  
    shader.setUniform("MVP", projection * view * model);
    if (normaalitMukana)
    {
    shader.setUniform("normalMatrix", glm::inverseTranspose(glm::mat3(model)));
    shader.setUniform("M", model);
    }
  //  shader.setUniform("V", view);
  //  shader.setUniform("P", projection);
  
    glDrawArrays(GL_TRIANGLES, 0, 24 /* triangleCount */);
  }

//  for (int i=0; i<10; i++)
//  {
//
//    float a = 0.0f;
//    float b = 0.0f;
//    float c = 0.0f;
//    float d = 0.0f;
//    if (i % 2 == 0)
//    {
//       a = yhyy2;
//       b = (i*0.5f)*yhyy3;
//       c = yhyy*(1.5f*i);
//       d = yhyy3*(0.0f*i);
//
//    }
//
//    else
//    {
//       a = (-1.0f)*yhyy2+(i*1.0f);
//       b = (-1.2f*i)*yhyy3;
//       c = yhyy + (i * 1.4f);
//       d = yhyy2 + yhyy3 + (1.3f * i);
//
//    }
//    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
//    glm::mat4 view = glm::lookAt(eyePosition, glm::vec3(0.0,0.0,0.0), glm::vec3(0.0f, 1.0f,  0.0f));
//    //glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f)); // glm::mat4(yhyy*0.2f);
//    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
//    glm::mat4 rotation = glm::rotate(b*180.0f, glm::vec3(1.0f,c*1.0f,d*1.0f));
//    glm::mat4 model = glm::translate(rotation * scale, glm::vec3(a*3.0f,c*3.0f, d*3.0f)); // glm::mat4(yhyy*0.2f);
//    shader.setUniform("normalMatrix", glm::inverseTranspose(glm::mat3(model)));
//  
//    shader.setUniform("MVP", projection * view * model);
//    shader.setUniform("M", model);
//  //  shader.setUniform("V", view);
//  //  shader.setUniform("P", projection);
//  
//    glDrawArrays(GL_TRIANGLES, 0, 36);
//  }
}
