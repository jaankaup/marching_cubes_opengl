#include "modelManager.h"

ModelManager::ModelManager()
{

}

ModelManager::~ModelManager()
{
//   for (const auto& data : pModels)
//   {
//     std::get<1>(data).dispose();
//   }
}

Model* ModelManager::createModel(const std::string& name)
{
  Model s;
//  s.init();
  auto modelData = std::make_tuple(name,s);
  pModels.push_back(modelData);
  return getModelByName(name);
}

ModelManager& ModelManager::getInstance()
{
    static ModelManager instance;
    return instance;
}

Model* ModelManager::getModelByName(const std::string& name) const
{
    for (const auto& obj : pModels)
    {
        if (std::get<0>(obj) == name)
        {
            return const_cast<Model*>(&std::get<1>(obj));
        }
    }
    throw std::runtime_error("ModelManager::getModelByName(" + name + "). No such model.");
}

Model* ModelManager::create_green_thing(bool wireframe)
{

  glm::mat4 original = glm::mat4(1.0f);

  const int BLOCK_SIZE = 8;
  const int CUBE_COUNT_X = BLOCK_SIZE * 8 ;
  const int CUBE_COUNT_Y = BLOCK_SIZE * 8  ;
  const int CUBE_COUNT_Z = BLOCK_SIZE * 8 ;
  const int CUBE_TOTAL_COUNT = CUBE_COUNT_X * CUBE_COUNT_Y * CUBE_COUNT_Z;

  const std::string TEXTURE_NAME = "greenThingTexture"; 
  const std::string VB_NAME = "greenThingVB"; 

  int vb_count = 0;

  if (wireframe)
  {
    Model* original = getModelByName("greenThingModel");
    std::vector<Command>& gm = *(original->getCommands());
    Command c = gm[0]; 
    c.shaderName = "marchingShaderWire_green";
    original->addCommand(c);
    return original;
  }

  if (!wireframe)
  {
////  Log::getDebug().log("creating the green optimization shader");
////  // The shader for optimizing the green thing data.
////  Shader* green_optimization_shader1 = ShaderManager::getInstance().createShader("green_thing_optimized");
//////  green_optimization_shader1.
////  std::vector<std::string> green_stage1 = {"shaders/marching.vert", "shaders/triangulate.geom"};
////  green_optimization_shader1->setFeedback(true,"outputCase");
////  green_optimization_shader1->build(green_stage1);
  }

  if (!wireframe)
  {
    // Create the 3D texture data.
//    Texture tex3D = TextureManager::getInstance().create3D(TEXTURE_NAME);
////    auto tex3D_data = createRandom3Ddata(CUBE_COUNT_X*2,CUBE_COUNT_Y*2,CUBE_COUNT_Z*2);
//    auto tex3D_data = createPerlin3D(CUBE_COUNT_X*2,CUBE_COUNT_Y*2,CUBE_COUNT_Z*2);
//    tex3D.create3D(tex3D_data);

    auto vb = VertexBufferManager::getInstance().createVertexBuffer(VB_NAME);
    vb_count = vb->createExamplePoints(CUBE_COUNT_X, CUBE_COUNT_Y, CUBE_COUNT_Z);
  }
  else vb_count = VertexBufferManager::getInstance().getVertexBufferByName(VB_NAME)->getCount(); 

  Log::getDebug().log("creating GREEEEEEEN");
  Model m;
  m.setDisabled(false);
  m.setCameraPosition(glm::vec3(0.0f,0.0f,17.0f));
  Command c;
//  c.block_size = BLOCK_SIZE;
//  c.cube_count_x = CUBE_COUNT_X;
//  c.cube_count_y = CUBE_COUNT_Y;
//  c.cube_count_z = CUBE_COUNT_Z;
//  c.start_point = glm::vec3(-CUBE_COUNT_X/2.0f,-CUBE_COUNT_Y/2.0f,-CUBE_COUNT_Z/2.0f);
  c.vao = VertexBufferManager::getInstance().getVertexBufferByName(VB_NAME)->getVAO(); 
  c.draw = GL_POINTS;
//////  c.textureName = TEXTURE_NAME;
 // if (wireframe) c.shaderName = "marchingShaderLine";
  c.shaderName = "marchingShader_green";
  c.startIndex = 0;
  c.count = vb_count;
  c.modelMatrix = glm::mat4(1.0f);
  m.addCommand(c);
//  std::string modelName;
//  if (!wireframe) modelName = "greenThingModel";
//  else  modelName = "greenThingModel_line";
  auto modelData = std::make_tuple("greenThingModel",m);
  pModels.push_back(modelData);
  return getModelByName("greenThingModel");
}

const std::vector<Model*> ModelManager::getModels() const
{
  std::vector<Model*> result;
   for (const auto& data : pModels)
   {
     result.push_back(const_cast<Model*>(&std::get<1>(data)));
   }
   return result;
}
