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
}

void Renderer::render(const Vertexbuffer& vb,
                      const Shader& shader,
                      int triangleCount,
                      const glm::mat4& viewMatrix,
                      const glm::vec3& eyePosition
//                      const glm::vec3& lightPosition,
//                      const glm::vec3& eyePosition,
//                      const glm::mat4& mvp,
//                      const Window& window
                      )
{
  bool normaalitMukana = true;
  //lkm++;
  glClearColor(0.0f,0.0f,0.0f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  vb.bind();
  shader.bind();
  //vb.bind();
  if (normaalitMukana)
  {
//  shader.setUniform("lights[0].position", lightPosition);
  shader.setUniform("lights[0].color", glm::vec3(1.0f,1.0f,1.0f));
  shader.setUniform("lights[0].ambientCoeffience", 0.25f);
  shader.setUniform("lights[0].materialSpecularColor", glm::vec3(1.0f,1.0f,1.0f));
  shader.setUniform("lights[0].materialShininess", 70.0f);
  shader.setUniform("lights[0].attentuationFactor", 0.00009f);
  //shader.setUniform("lights[0].attentuationFactor", 0.000009f);
  }

    //glm::vec3 eyePosition = glm::vec3(yhyy*40.0f + (-1.0f)*25.0f,yhyy3*50.0f + 25.0f, yhyy2*50.0f + 25.0f);
//    glm::vec3 eyePosition = glm::vec3(yhyy*200.0f + 100.0f,50.0f, 150.0f);
    shader.setUniform("cameraPosition", eyePosition);
    shader.setUniform("lights[0].position", eyePosition);
//    Log::getDebug().log("lights[0].position(%,%,%)", std::to_string(eyePosition.x),std::to_string(eyePosition.y),std::to_string(eyePosition.z));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 700.0f);
//    glm::mat4 view = glm::lookAt(eyePosition, glm::vec3(0.0,0.0,0.0), glm::vec3(0.0f, 1.0f,  0.0f));
    //glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f)); // glm::mat4(yhyy*0.2f);
//    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
//    glm::mat4 rotation = glm::rotate(a*180.0f, glm::vec3(1.0f,b*1.0f,c*1.0f));
//    glm::mat4 model = glm::translate(rotation * scale, glm::vec3(b*2.0f,c*2.0f, d*2.0f)); // glm::mat4(yhyy*0.2f);
    glm::mat4 model = glm::mat4(1.0f); // glm::mat4(yhyy*0.2f);
  
    shader.setUniform("MVP", projection * viewMatrix * model);
    if (normaalitMukana)
    {
    shader.setUniform("normalMatrix", glm::inverseTranspose(glm::mat3(model)));
    shader.setUniform("M", model);
    }
  //  shader.setUniform("V", view);
  //  shader.setUniform("P", projection);
  
    // 0 (offset taulukossa). piirrettava kolmiomaara ts. 3*trianglecount 
    glDrawArrays(GL_TRIANGLES, 0, 3 * triangleCount );
  }
