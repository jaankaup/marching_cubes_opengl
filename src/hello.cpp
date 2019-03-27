#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emscripten.h>
#include <math.h>
#include <vector>

#include "Graphics/window.h"
#include "Graphics/shader.h"
#include "Graphics/model.h"
#include "Graphics/vertexbuffer.h"
#include "Graphics/indexbuffer.h"
#include "Graphics/vertexAttributes.h"
#include "Graphics/renderer.h"
#include "Graphics/texture.h"
#include "Graphics/testData.h"
#include "Graphics/cell.h"
#include "Graphics/marchingCubes.h"
#include "Graphics/voxeldata.h"
#include "Graphics/camera.h"
#include "Graphics/textureManager.h"
#include "Graphics/shaderManager.h"
#include "Graphics/model.h"
#include "Utils/log.h"
#include "Utils/kokeilu.h"

//struct SDL_Window;
//enum TextureType : int32_t;

/**
 * Context structure that will be passed to the loop handler
 */
struct context
{
    Renderer renderer;
    Vertexbuffer vertexbuffer;
    int triangleCount;
    Camera camera;
    std::vector<Model> models;
    Shader shader = ShaderManager::getInstance().createShader("my3Dshader");
    Texture texture = TextureManager::getInstance().create3D("my3Dtexture");//{TextureType::d2,0};
};

void loop_handler2(void *arg)
{
    context* c = static_cast<context*>(arg);
    //c->texture.bind();
    
    c->camera.handleEvents();
    auto viewMatrix = c->camera.getViewMatrix();
    c->renderer.render(c->vertexbuffer,ShaderManager::getInstance().getShaderByName("my3Dshader"),c->triangleCount,viewMatrix,c->camera.getPosition());
//    c->renderer.renderModels(c->models,c->camera);
    Window::getInstance().swapBuffers();
}

int main()
{
  Window window = Window::getInstance();
//  Shader shader = ShaderManager::getInstance().createShader("my3Dshader");
//  Texture texture = TextureManager::getInstance().create3D("my3Dtexture");//{TextureType::d2,0};
  context c;
  std::vector<std::string> shaderSources = {"shaders/marching.vert", "shaders/marching.frag"};
  Vertexbuffer vb;
  Renderer r;

  c.camera = Camera();

  c.renderer = r; 
  c.renderer.init();

  c.vertexbuffer = std::move(vb); 
  c.vertexbuffer.init();
  c.vertexbuffer.createExampleCube();
  c.triangleCount = 6*2;

//  c.shader = s; 
//  c.shader.init();
  c.shader.build(shaderSources);
  c.shader.bind();
  //shader.build(shaderSources);
  //shader.bind();
  
  //Texture t(TextureType::d3, 0);
  //c.texture = t;
  //c.texture.init();
  //c.texture.createExample2D();//("assets/rock.jpg");
  c.texture.create3D();//("assets/rock.jpg");
  //texture.create3D();//("assets/rock.jpg");
  //c.texture.create("assets/rock.jpg");
//  c.texture.use3D(0);
  c.shader.setUniform("diffuse3DTexture",0);
  //shader.setUniform("diffuse3DTexture",0);
////  Model m;
//////  m.addModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
////  Command command;
////  command.vao = c.vertexbuffer.getVAO();
////  command.textureName = "my3Dtexture";
////  command.shaderName = "my3Dshader";
////  command.startIndex = 0;
////  command.count = 12;
////  command.modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
////  m.addCommand(command);
////  c.models.push_back(m);

////////  auto tData = exampleData2();
////////
//////////std::vector<glm::vec3> triangulate(const ArrayType& data, float isolevel)
////////  auto [vertices,normals] = triangulate(tData, 0.5f);
////////  std::vector<glm::vec3> marchingData;
////////  for (int q=0; q<vertices.size() ; q++)
////////  {
////////    marchingData.push_back(vertices[q]);
////////    Log::getDebug().log("vertice = (%,%,%)", std::to_string(vertices[q].x),std::to_string(vertices[q].y),std::to_string(vertices[q].z));
////////    marchingData.push_back(normals[q]);
////////    Log::getDebug().log("normal = (%,%,%)", std::to_string(normals[q].x),std::to_string(normals[q].y),std::to_string(normals[q].z));
//////////    auto n = glm::vec3(0.0f,0.0f,0.0f);
//////////    marchingData.push_back(n);
////////
///////////    Log::getDebug().log("(%,%,%)", std::to_string(x.x), std::to_string(x.y),std::to_string(x.z));
////////  }
//  Log::getDebug().log("size of tData = %", std::to_string(tData.size()));
//  Log::getDebug().log("size of vertices = %", std::to_string(vertices.size()));
//  Log::getDebug().log("size of normals/3 = %", std::to_string(normals.size()/3));
//  for (const auto& e : res)
//  {
////    std::cout << x << std::endl;
//    Log::getDebug().log("(%,%,%)", std::to_string(e.x), std::to_string(e.y),std::to_string(e.z));
//  }
  
//  Log::getDebug().log("size of tData = %", std::to_string(tData.size()));
//  for (int k=0 ; k<3 ; k++)
//  {
//  for (int j=0 ; j<3 ; j++)
//  {
//  for (int i=0 ; i<3 ; i++)
//  {
//    Log::getDebug().log("vData.getValue(%,%,%) = %", std::to_string(i), std::to_string(j),std::to_string(k),std::to_string(tData.getValue(i,j,k)));
//    Log::getDebug().log("index = %", std::to_string(i+4*j+16*k));
//  }}}
////  c.vertexbuffer.addData(&marchingData[0], marchingData.size() * sizeof(glm::vec3));
////
////  c.triangleCount = vertices.size()/3;
////  //ShaderManager::getInstance().createShader(shaderSources2, "pah");
////  Log::getDebug().log("triangleCount = %", std::to_string(c.triangleCount));
//    SDL_Window *window;
//    struct context ctx;

//    SDL_Init(SDL_INIT_VIDEO);
//    SDL_CreateWindowAndRenderer(600, 400, 0, &window, &ctx.renderer);
//    SDL_SetRenderDrawColor(ctx.renderer, 255, 255, 255, 255);
//
//    get_owl_texture(&ctx);
//    ctx.active_state = NOTHING_PRESSED;
//    ctx.dest.x = 200;
//    ctx.dest.y = 100;
//    ctx.owl_vx = 0;
//    ctx.owl_vy = 0;

    /**
     * Schedule the main loop handler to get 
     * called on each animation frame
     */
    emscripten_set_main_loop_arg(loop_handler2, &c, -1, 1);

//    c.window.dispose();
    return 0;
}

