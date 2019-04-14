#include <stdio.h>
#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
#include <GL/glew.h>

#ifdef EMSCRIPTEN
  #include <emscripten.h>
#endif

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
#include "Graphics/programstate.h"
#include "Utils/log.h"
#include "Utils/misc.h"
#include "Utils/myrandom.h"


/**
 * Context structure that will be passed to the loop handler
 */
struct context
{
    Renderer renderer;
    Vertexbuffer vertexbuffer;
    Vertexbuffer vertexbuffer2;
    Vertexbuffer vertexbuffer3;
    Camera camera;
    std::vector<Model> models;
};

void loop_handler2(void *arg)
{
    context* c = static_cast<context*>(arg);
    c->camera.handleEvents();
    c->renderer.renderModels(c->models,c->camera);
    Window::getInstance().swapBuffers();
}

int main()
{

  const int BLOCK_SIZE = 16;

  // 3D texture height.
  const int SAMPLE_POINTS_Y = BLOCK_SIZE ;

  const int CUBE_COUNT_X = BLOCK_SIZE * 2;
  const int CUBE_COUNT_Y = BLOCK_SIZE * 1;
  const int CUBE_COUNT_Z = BLOCK_SIZE * 2;
  const int CUBE_TOTAL_COUNT = CUBE_COUNT_X * CUBE_COUNT_Y * CUBE_COUNT_Z;

  // The program state must be created first.
  ProgramState::getInstance();
  
  // Set base cube dimension.
//  ProgramState::getInstance().setCubeDimension(glm::ivec3(CUBE_COUNT_X*2,CUBE_COUNT_Y*2,CUBE_COUNT_Z*2));

  // Initialize the voxelsPerBlock count.
  ProgramState::getInstance().setVoxelsPerBlock(float(BLOCK_SIZE)); 

  // Set the starting point to 0.25 height of the cube height.
  auto y_start = static_cast<float>(SAMPLE_POINTS_Y);  
  ProgramState::getInstance().setStartPoint(glm::vec3(-CUBE_COUNT_X/2.0f, -CUBE_COUNT_Y/2.0f /*  CUBE_COUNT_Y/4.0f    y_start */ ,-CUBE_COUNT_Z/2.0f));
  
//  logGLM("sp",ProgramState::getInstance().getStartPoint());

//  ProgramState::getInstance().setVoxelsPerBlock(8.0f); 
  // Create the window.
  Window window = Window::getInstance();

  // The shader for shading textured cube.
  Shader shaderCube = ShaderManager::getInstance().createShader("cubeShader");

  // 3D texture.
  Texture texture = TextureManager::getInstance().create3D("my3Dtexture");

  // The tri_table data .
  Texture tritable = TextureManager::getInstance().create1D("tri_table_texture");
  tritable.create_tritable_texture();

  // Creates a default texture for rendering the cube.
  Texture textureCube = TextureManager::getInstance().create2D("cubeTexture");

  // The shader sources for the default 2D texture shader.
  std::vector<std::string> shaderSourcesCube = {"shaders/default.vert", "shaders/default.frag"};

  // The default shader compilation.
  shaderCube.build(shaderSourcesCube);

  // Context creation.
  context c;

  // We create marching cubes shader only with native opengl.
  #ifndef EMSCRIPTEN
    Shader marchingShader = ShaderManager::getInstance().createShader("marchingShader");
    std::vector<std::string> marchingShader_src = {"shaders/marching.vert", "shaders/marching.geom", "shaders/marching.frag"};
    marchingShader.build(marchingShader_src);

    Shader marchingShaderLine = ShaderManager::getInstance().createShader("marchingShaderLine");
    std::vector<std::string> marchingShaderLine_src = {"shaders/marching.vert", "shaders/marchingWireFrame.geom", "shaders/marchingLine2.frag"};
    marchingShaderLine.build(marchingShaderLine_src);
  #endif

  // The initialization of creation of renderer.
  c.renderer.init();

  // Creation of the cube.
  c.vertexbuffer.init();
  c.vertexbuffer.createExampleCube();

  // The triangle count of the cube.
//  c.triangleCount = 6*2*3;

  c.vertexbuffer2.init();
  int vb2_count = c.vertexbuffer2.createExamplePoints(CUBE_COUNT_X, CUBE_COUNT_Y, CUBE_COUNT_Z);

  c.vertexbuffer3.init();
  int vb3_count = c.vertexbuffer3.createExamplePointsTier2(CUBE_COUNT_X, CUBE_COUNT_Y, CUBE_COUNT_Z);
//  int vb3_count = c.vertexbuffer3.createExamplePoints(CUBE_COUNT_X, CUBE_COUNT_Y, CUBE_COUNT_Z);

  // Create the 3D texture data.
  auto hyh = createRandom3Ddata(BLOCK_SIZE*2,BLOCK_SIZE*2,BLOCK_SIZE*2);
  //auto hyh = createRandom3Ddata(CUBE_COUNT_X*2,CUBE_COUNT_Y*2,CUBE_COUNT_Z*2);
//    auto hyh = create2x2();
  texture.create3D(hyh);
  textureCube.create("assets/rock.jpg");

////  Model m;
//////  m.addModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
////  Command command;
////  command.vao = c.vertexbuffer.getVAO();
////  command.textureName = "my3Dtexture";
////  command.shaderName = "my3Dshader";
////  command.startIndex = 0;
////  command.count = 12*3;
////  command.modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
////  m.addCommand(command);
////  c.models.push_back(m);


  #ifndef EMSCRIPTEN

  glm::mat4 original = glm::mat4(1.0f);

  Shader geom = ShaderManager::getInstance().getShaderByName("marchingShader");
  Model m;
  Command command;
  command.vao = c.vertexbuffer2.getVAO();
  command.draw = GL_POINTS;
  //command.textureName = "my3Dtexture";
  command.textureName = "my3Dtexture";
  command.shaderName = "marchingShader";
  command.startIndex = 0;
  command.count = vb2_count; // CUBE_TOTAL_COUNT;
//  auto scale = glm::scale(original,glm::vec3(58.0f));
//  auto rotate = glm::rotate(original,glm::radians(0.0f),glm::vec3(1.0f,0.0f,0.0f));
//  auto translate = glm::translate(original,glm::vec3(0.0f,-0.5f,-1.0f));
//  command.modelMatrix = scale * translate * rotate;
  command.modelMatrix = original;
  m.addCommand(command);
  c.models.push_back(m);

  Model m3;
  Command command3;
  command3.vao = c.vertexbuffer2.getVAO();
  command3.draw = GL_POINTS;
  //command.textureName = "my3Dtexture";
  command3.textureName = "my3Dtexture";
  command3.shaderName = "marchingShaderLine";
  command3.startIndex = 0;
  command3.count = vb2_count; //CUBE_TOTAL_COUNT;
//  command3.modelMatrix = scale * translate * rotate;
  command3.modelMatrix = original;
  m3.addCommand(command3);
  c.models.push_back(m3);

  Model m5;
  Command command5;
  command5.name = "marching_tier2_wire";
  command5.vao = c.vertexbuffer3.getVAO();
  command5.draw = GL_POINTS;
  //command.textureName = "my3Dtexture";
  command5.textureName = "my3Dtexture";
  command5.shaderName = "marchingShaderLine";
  command5.startIndex = 0;
  command5.count = vb3_count; //CUBE_TOTAL_COUNT;
//  command3.modelMatrix = scale * translate * rotate;
//  auto scale5 = glm::scale(original,glm::vec3(1.0f));
//  auto rotate5 = glm::rotate(original,glm::radians(0.0f),glm::vec3(1.0f,0.0f,0.0f));
//  auto translate5 = glm::translate(original,glm::vec3(-2.0f,-2.0f,-2.0f));
//  command5.modelMatrix = translate5;
  command5.modelMatrix = original;
  m5.addCommand(command5);
  c.models.push_back(m5);

  Model m4;
  Command command4;
  command4.name = "marching_tier2";
  command4.vao = c.vertexbuffer3.getVAO();
  command4.draw = GL_POINTS;
  //command.textureName = "my3Dtexture";
  command4.textureName = "my3Dtexture";
  command4.shaderName = "marchingShader";
  command4.startIndex = 0;
  command4.count = vb3_count; // 16859136;
//  command3.modelMatrix = scale * translate * rotate;
  command4.modelMatrix = original;
  m4.addCommand(command4);
  c.models.push_back(m4);
  #endif

  Model m2;
//  m.addModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
  Command command2;
  command2.vao = c.vertexbuffer.getVAO();
  command2.draw = GL_TRIANGLES;
  command2.textureName = "cubeTexture";
  command2.shaderName = "cubeShader";
  command2.startIndex = 0;
  command2.count = 12*3;
  glm::mat4 original2 = glm::mat4(1.0f);
  auto scale2 = glm::scale(original2,glm::vec3(1.0f));
  auto rotate2 = glm::rotate(original2,glm::radians(30.0f),glm::vec3(1.0f,0.0f,0.0f));
  auto translate2 = glm::translate(original2,glm::vec3(8.0f,8.0f,8.0f));
  command2.modelMatrix = scale2 * translate2 * rotate2;
  m2.addCommand(command2);
  c.models.push_back(m2);

//  Log::getDebug().log("GL_GEOMETRY_SHADER = %", std::to_string(GL_GEOMETRY_SHADER));
  
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


    /**
     * Schedule the main loop handler to get 
     * called on each animation frame
     */

    #ifdef EMSCRIPTEN
    emscripten_set_main_loop_arg(loop_handler2, &c, -1, 1);
    #endif

    #ifndef EMSCRIPTEN
    while (ProgramState::getInstance().getAppRunning())
    {
      loop_handler2(&c);
    }
    #endif

//    c.window.dispose();
    return 0;
}

