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
#include "Graphics/timer.h"
#include "Graphics/vertexBufferManager.h"
#include "Graphics/modelManager.h"
#include "Graphics/camera2.h"
#include "Utils/log.h"
#include "Utils/misc.h"
#include "Utils/myrandom.h"


/**
 * Context structure that will be passed to the loop handler
 */
struct context
{
    Renderer renderer;
    Camera2 camera = Camera2(glm::vec3(5.0f,7.0f,5.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
};

void createShaders()
{
    
    // TODO: delete all existing shaders here.
    
// WE OMIT THE DEBUG SHADERS FOR NOW. 
  
//    const std::string MARCHING_CUBES_SHADER = "marchingShader_green"; 
//    const std::string MARCHING_CUBES_WIREFRAME = "marchingShaderWire_green"; 
    const std::string MARCHING_CUBES_TRIANGULATION = "triangulationShader"; 
    const std::string SCENE_SHADER = "cubeshader"; 

//    // The marching cubes shader.
//    Shader* marchingShader = ShaderManager::getInstance().createShader(MARCHING_CUBES_SHADER);
//    std::vector<std::string> marchingShader_src = {"shaders/marching.vert", "shaders/marching_green_thing.geom", "shaders/marching.frag"};
//    marchingShader->build(marchingShader_src);
//
//    ProgramState::getInstance().getMetadata()->cubeMarchShader = MARCHING_CUBES_SHADER;
//
//    // The marching cubes wireframe shader.
//    Shader* marchingShaderLine = ShaderManager::getInstance().createShader(MARCHING_CUBES_WIREFRAME);
//    std::vector<std::string> marchingShaderLine_src = {"shaders/marching.vert", "shaders/marchingWireFrame_green.geom", "shaders/marchingLine2.frag"};
//    marchingShaderLine->build(marchingShaderLine_src);
//
//    ProgramState::getInstance().getMetadata()->cubeMarchWireframe = MARCHING_CUBES_WIREFRAME;

    // The triangulation shader with density function 0-9 .
    for (int i=0 ; i < 10 ; i++)
    {
      Shader* triangulationShader = ShaderManager::getInstance().createShader(MARCHING_CUBES_TRIANGULATION + std::to_string(i));
      std::vector<std::string> triangulate_src = {"shaders/triangulate.vert", "shaders/triangulate.geom", "shaders/densityFunction" + std::to_string(i) + ".df"};
      triangulationShader->setFeedback(true,"outputCase");
      triangulationShader->buildDensity(triangulate_src);
    }

    ProgramState::getInstance().getMetadata()->triangulationShader = MARCHING_CUBES_TRIANGULATION + std::to_string(1);

    // The shader for drawing the triangulated scene. The name is a bit
    // misleading.
    Shader* shaderCube = ShaderManager::getInstance().createShader(SCENE_SHADER);
    std::vector<std::string> shaderSourcesCube = {"shaders/default_notex.vert", "shaders/default_notex.frag"};
    shaderCube->build(shaderSourcesCube,false);

    ProgramState::getInstance().getMetadata()->meshShader = SCENE_SHADER;
}

// Initialize the marching cubes attributes.
void initializeCubeAttributes()
{
  const int BLOCK_SIZE = 32;
  const int CUBE_COUNT_X = BLOCK_SIZE;
  const int CUBE_COUNT_Y = BLOCK_SIZE;
  const int CUBE_COUNT_Z = BLOCK_SIZE;
  const float ISO_VALUE = 0.0f;

  auto metadata = ProgramState::getInstance().getMetadata();
  metadata->block_size = BLOCK_SIZE; 
  metadata->cube_count_x = CUBE_COUNT_X;
  metadata->cube_count_y = CUBE_COUNT_Y;
  metadata->cube_count_z = CUBE_COUNT_Z;
  metadata->isovalue = ISO_VALUE;
}

void createtextures()
{
  const std::string TEXTURE_NAME = "greenThingTexture"; 
  const std::string TRITABLE_NAME = "tri_table_texture"; 

  auto metadata = ProgramState::getInstance().getMetadata();

  // Create the 3D texture.
  Texture tex3D = TextureManager::getInstance().create3D(TEXTURE_NAME);
  auto tex3D_data = createPerlin3D(metadata->block_size*2,metadata->block_size*2,metadata->block_size*2);
  tex3D.create3D(tex3D_data);
  metadata->texture3Dname = TEXTURE_NAME;

  // Create the tri_table.
  Texture tritable = TextureManager::getInstance().create1D(TRITABLE_NAME);
  tritable.create_tritable_texture();
  metadata->tri_table_name = TRITABLE_NAME;// TRITABLE_NAME;
}

void loop_handler2(void *arg)
{
    context* c = static_cast<context*>(arg);
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            //running_ = false;
            break;

        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
                case SDLK_1:
                    changeScene(1);
                    break;
                case SDLK_2:
                    changeScene(2);
                    break;
                case SDLK_3:
                    changeScene(3);
                    break;
                case SDLK_4:
                    changeScene(4);
                    break;
                case SDLK_5:
                    changeScene(5);
                    break;
                case SDLK_6:
                    changeScene(6);
                    break;
                case SDLK_7:
                    changeScene(7);
                    break;
                case SDLK_8:
                    changeScene(8);
                    break;
                case SDLK_9:
                    changeScene(9);
                    break;
                case SDLK_0:
                    changeScene(0);
                    break;
//                case SDLK_SPACE: /* Kamera */
//                    camera_.toggleMode();
//                    break;

                case SDLK_q: 
                    ProgramState::getInstance().setAppRunning(false);
                    break;
                case SDLK_ESCAPE: 
                    ProgramState::getInstance().setAppRunning(false);
                    break;
                case SDLK_t: 
                    auto metadata = ProgramState::getInstance().getMetadata();
                    auto name = metadata->texture3Dname;
                    TextureManager::getInstance().deleteTexture(name); 
                    Texture tex3D = TextureManager::getInstance().create3D(name);
                    auto tex3D_data = createPerlin3D(metadata->block_size*2,metadata->block_size*2,metadata->block_size*2);
                    tex3D.create3D(tex3D_data);
                    metadata->texture3Dname = name;
                    Log::getInfo().log("Creating a new 3D texture...");
                    // Recreate scene model.
                    Log::getInfo().log("Rebuilding scene...");
                    ModelManager::getInstance().createSceneObject();
                    break;
            }
        case SDL_WINDOWEVENT:
            switch(e.window.event)
            {
              case SDL_WINDOWEVENT_CLOSE:
              //    running_ = false;
                  break;
              case SDL_WINDOWEVENT_RESIZED:
              {
                  int w = e.window.data1;
                  int h = e.window.data2;
                  glViewport(0,0,w,h);
                  break;
              }
            }

        /* Kameralle */
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
            c->camera.handleMouseInput(e);
            break;

        }
    }
    c->camera.handleKeyInput();
    c->renderer.renderModels(c->camera);
    Window::getInstance().swapBuffers();
}

//int main()
int main(int argc, char* argv[])
{

  // The program state must be created first.
  ProgramState::getInstance();
  
  // Initialize the base information for the marching cubes.
  initializeCubeAttributes();

  // Create the window. This also creates the opengl context.
  Window window = Window::getInstance();

  // Create all textures.
  createtextures();

  // Creates a default texture for rendering the cube.
  Texture textureCube = TextureManager::getInstance().create2D("cubeTexture");

  // Context creation. (webassembly).
  context c;

  // We create marching cubes shader only with native opengl.
    
  #ifndef EMSCRIPTEN

  createShaders();

///////    ProgramState::getInstance().getMetadata()->cubeMarchWireframe = MARCHING_CUBES_WIREFRAME;
  #endif

  // Initialize the renderer.
  c.renderer.init();

  #ifndef EMSCRIPTEN

  // Create models.
//  Model* green_thing = ModelManager::getInstance().create_green_thing(false);
////  Model* green_thing_line = ModelManager::getInstance().create_green_thing(true);
//  std::vector<Command>& gm = *(green_thing->getCommands());
////  Command c_green;
//  if (gm.size() == 0) Log::getError().log("hello::main. gm.size() = %", std::to_string(gm.size()));
//  auto mData = ProgramState::getInstance().getMetadata();
//  //Vertexbuffer* green_vb = VertexBufferManager::getInstance().getVertexBufferByName("greenThingVB");
//  Vertexbuffer* optimizedGreen =
//    VertexBufferManager::getInstance().optimize_vertex_buffer("green_thing_optimized_vb",
//                                                              "green_thing_optimized");
//  auto ps = ProgramState::getInstance();
//  gm[0].vao = optimizedGreen->getVAO();
//  gm[0].count = optimizedGreen->getCount();
//  gm[0].shaderName = ps.getMetadata()->meshShader;
//  gm[0].draw = GL_TRIANGLES;
    ModelManager::getInstance().createSceneObject();
//  glm::mat4 original = glm::mat4(1.0f);
//
//  Shader geom = ShaderManager::getInstance().getShaderByName("marchingShader");
//  Model m;
//  Command command;
//  command.vao = vb2.getVAO();
//  command.draw = GL_POINTS;
//  //command.textureName = "my3Dtexture";
//  command.textureName = "my3Dtexture";
//  command.shaderName = "marchingShader";
//  command.startIndex = 0;
//  command.count = vb2_count; // CUBE_TOTAL_COUNT;
////  auto scale = glm::scale(original,glm::vec3(58.0f));
////  auto rotate = glm::rotate(original,glm::radians(0.0f),glm::vec3(1.0f,0.0f,0.0f));
////  auto translate = glm::translate(original,glm::vec3(0.0f,-0.5f,-1.0f));
////  command.modelMatrix = scale * translate * rotate;
//  command.modelMatrix = original;
//  m.addCommand(command);
//  c.models.push_back(m);
//
//  Model m3;
//  Command command3;
//  command3.vao = vb2.getVAO();
//  command3.draw = GL_POINTS;
//  //command.textureName = "my3Dtexture";
//  command3.textureName = "my3Dtexture";
//  command3.shaderName = "marchingShaderLine";
//  command3.startIndex = 0;
//  command3.count = vb2_count; //CUBE_TOTAL_COUNT;
////  command3.modelMatrix = scale * translate * rotate;
//  command3.modelMatrix = original;
//  m3.addCommand(command3);
//  c.models.push_back(m3);
//
//  Model m5;
//  Command command5;
//  command5.name = "marching_tier2_wire";
//  command5.vao = vb3.getVAO();
//  command5.draw = GL_POINTS;
//  //command.textureName = "my3Dtexture";
//  command5.textureName = "my3Dtexture";
//  command5.shaderName = "marchingShaderLine";
//  command5.startIndex = 0;
//  command5.count = vb3_count; //CUBE_TOTAL_COUNT;
////  command3.modelMatrix = scale * translate * rotate;
////  auto scale5 = glm::scale(original,glm::vec3(1.0f));
////  auto rotate5 = glm::rotate(original,glm::radians(0.0f),glm::vec3(1.0f,0.0f,0.0f));
////  auto translate5 = glm::translate(original,glm::vec3(-2.0f,-2.0f,-2.0f));
////  command5.modelMatrix = translate5;
//  command5.modelMatrix = original;
//  m5.addCommand(command5);
//  c.models.push_back(m5);
//
//  Model m4;
//  Command command4;
//  command4.name = "marching_tier2";
//  command4.vao = vb3.getVAO();
//  command4.draw = GL_POINTS;
//  //command.textureName = "my3Dtexture";
//  command4.textureName = "my3Dtexture";
//  command4.shaderName = "marchingShader";
//  command4.startIndex = 0;
//  command4.count = vb3_count; // 16859136;
////  command3.modelMatrix = scale * translate * rotate;
//  command4.modelMatrix = original;
//  m4.addCommand(command4);
//  c.models.push_back(m4);
  #endif
//
//  Model m2;
////  m.addModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
//  Command command2;
//  command2.vao = vb.getVAO();
//  command2.draw = GL_TRIANGLES;
//  command2.textureName = "cubeTexture";
//  command2.shaderName = "cubeShader";
//  command2.startIndex = 0;
//  command2.count = 12*3;
//  glm::mat4 original2 = glm::mat4(1.0f);
//  auto scale2 = glm::scale(original2,glm::vec3(1.0f));
//  auto rotate2 = glm::rotate(original2,glm::radians(30.0f),glm::vec3(1.0f,0.0f,0.0f));
//  auto translate2 = glm::translate(original2,glm::vec3(8.0f,8.0f,8.0f));
//  command2.modelMatrix = scale2 * translate2 * rotate2;
//  m2.addCommand(command2);
//  c.models.push_back(m2);
//
//  Model m234 = createEarth();
  Timer::getInstance().reset();



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

