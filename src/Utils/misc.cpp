#include "misc.h"

void joopajoo()
{
  std::cout << "1234kljh1234lkjh" << std::endl;
}

std::string addPadding(const std::string& str, const uint32_t count, const char paddingChar)
{
  std::string result = str;
  if (count > str.size())
  {
    result.insert(0,count - str.size(), paddingChar);
  }
  return result;
}

//template <typename T>
//void logGLM(const std::string& name, const T& object) {}

void logGLM(const std::string& name, const glm::vec2& object)
{
  Log::getDebug().log("% = (%,%)",name,std::to_string(object.x),std::to_string(object.y));
}
   
void logGLM(const std::string& name, const glm::vec3& object)
{
  Log::getDebug().log("% = (%,%,%)",name,std::to_string(object.x),std::to_string(object.y),std::to_string(object.z));
}

void logGLM(const std::string& name, const glm::vec4& object)
{
  Log::getDebug().log("% = (%,%,%,%)",name,std::to_string(object.x),std::to_string(object.y),std::to_string(object.w));
}

void logGLM(const std::string& name, const glm::mat3& object)
{
  std::string padded = addPadding("",name.size(),' ');
  Log::getDebug().log("% = (%,%,%)",name   ,std::to_string(object[0][0]),std::to_string(object[0][1]),std::to_string(object[0][2]));
  Log::getDebug().log("%   (%,%,%)",padded ,std::to_string(object[1][0]),std::to_string(object[1][1]),std::to_string(object[1][2]));
  Log::getDebug().log("%   (%,%,%)",padded ,std::to_string(object[2][0]),std::to_string(object[2][1]),std::to_string(object[2][2]));
}

void logGLM(const std::string& name, const glm::mat4& object)
{
  std::string padded = addPadding("",name.size(),' ');
  Log::getDebug().log("% = (%,%,%,%)",name   ,std::to_string(object[0][0]),std::to_string(object[0][1]),std::to_string(object[0][2]),std::to_string(object[0][3]));
  Log::getDebug().log("%   (%,%,%,%)",padded ,std::to_string(object[1][0]),std::to_string(object[1][1]),std::to_string(object[1][2]),std::to_string(object[1][3]));
  Log::getDebug().log("%   (%,%,%,%)",padded ,std::to_string(object[2][0]),std::to_string(object[2][1]),std::to_string(object[2][2]),std::to_string(object[2][3]));
  Log::getDebug().log("%   (%,%,%,%)",padded ,std::to_string(object[3][0]),std::to_string(object[3][1]),std::to_string(object[3][2]),std::to_string(object[3][3]));
}


TextureData createPerlin3D(const int width, const int height, const int depth)
{
  Log::getDebug().log("Persin noise w,h,d = (%,%,%)", std::to_string(width), std::to_string(height), std::to_string(depth));
  int size = width*height*depth*4;
  TextureData td(size,width,height,depth);
  
  auto data = td.getData();

  // Reinterpret the one dimensional array as multi-dimensional array.
  //uint8_t (&multiArray)[width][height][depth][4] = *reinterpret_cast<uint8_t (*)[width][height][depth][4]>(data);

  MyRandom<double> mr;
  mr.setDistribution(0,250000.0);

  siv::PerlinNoise pn(mr());
  siv::PerlinNoise pn2(mr());

  const double fx = width; // / frequency;
  const double fy = height; // / frequency;
  const double fz = depth; // / frequency;


//  MyRandom<int> mr2;
//  mr2.setDistribution(0,255);

  unsigned int xOffset = 4;
  unsigned int yOffset = 4 * width;
  unsigned int zOffset = 4 * width * height;
  unsigned int lkm = 0;
  for (int z = 0; z < depth ; z++) {
  for (int y = 0; y < height ; y++){
  for (int x = 0; x < width ; x++) {
    int density = mr();
	unsigned int position = x*xOffset + y*yOffset + z*zOffset;
    data[position] = 123;
    data[position+1] = 100; // mr2();
    data[position+2] = pn2.noise0_1(x / fx, y / fy, z / fz) * 255; 
    data[position+3] = pn.noise0_1(x / fx, y / fy, z / fz) * 255;
	lkm++;
  }}};

  return std::move(td);
}


// Create the buffer for the base data points.
void createBaseVertexBuffer()
{
  const std::string BASE_VERTEX_BUFFER_NAME = "sceneVBO"; 

  auto metadata = ProgramState::getInstance().getMetadata();
  metadata->base_vertex_buffer_name = BASE_VERTEX_BUFFER_NAME;
  auto shaderName = metadata->triangulationShader; 
  if (shaderName == "") Log::getError().log("Misc::createBaseVertexBuffer: triangulation shader was not created.");
//  auto vb = VertexBufferManager::getInstance().(BASE_VERTEX_BUFFER_NAME);
  auto vb = VertexBufferManager::getInstance().optimize_vertex_buffer(BASE_VERTEX_BUFFER_NAME, shaderName );
  //vb->createExamplePoints(metadata->cube_count_x, metadata->cube_count_y, metadata->cube_count_z);
}

void changeScene(const char number)
{
        std::string shaderName = ProgramState::getInstance().getMetadata()->triangulationShader;
        std::string newName = shaderName.substr(0, shaderName.size()-1) + std::to_string(number);
        Log::getInfo().log("ShaderName == %", newName);
        ProgramState::getInstance().getMetadata()->triangulationShader = newName;
        Log::getInfo().log("Changing densityShader to % ...", newName);
        ModelManager::getInstance().createSceneObject();
}
