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

// Plug; fzf
TextureData createRandom3Ddata(const int width, const int height, const int depth)
{
  int size = width*height*depth*4;
  TextureData td(size,width,height,depth);
  auto data = td.getData();
//  auto texels = new unsigned char[size];
  bool hah = false;

  MyRandom<int> mr;
  mr.setDistribution(0,155);
//  Log::getDebug().log("% ",std::to_string(mr()));
//  Log::getDebug().log("% ",std::to_string(mr()));
//  Log::getDebug().log("% ",std::to_string(mr()));
//  Log::getDebug().log("% ",std::to_string(mr()));
//  Log::getDebug().log("% ",std::to_string(mr()));
  for (int i=0 ; i<width*height*depth ; i++)
  {
    data[i*4] = 65;// 0.5f; //   (i*1.0f/size)*255 < 255 ? (i*1.0f/size)*255 : 255;
    data[i*4+1] = i % 255; // 1.0f; //(i*1.0f/size) < 1.0f ? (i*1.0f/size) : 1.0f;
    data[i*4+2] = hah ? 13 : 99; // (i*1.0f/size)*255 < 255 ? (i*1.0f/size)*255 : 255;
    data[i*4+3] = mr();// (int)((i/float(size))*255); // mr(); 
    hah = !hah;
  }
  return std::move(td);
}

TextureData createChess3Ddata(const int width, const int height, const int depth)
{
  int size = width*height*depth*4;
  TextureData td(size,width,height,depth);
  auto data = td.getData();
//  auto texels = new unsigned char[size];
  bool hah = false;

  MyRandom<int> mr;
  mr.setDistribution(0,155);

  for (int i=0 ; i<width*height*depth ; i++)
  {
    auto value = mr();
    Log::getDebug().log("index %: value == % ",std::to_string(i),std::to_string(mr()));
    data[i*4] = 65;// 0.5f; //   (i*1.0f/size)*255 < 255 ? (i*1.0f/size)*255 : 255;
    data[i*4+1] = i % 255; // 1.0f; //(i*1.0f/size) < 1.0f ? (i*1.0f/size) : 1.0f;
    data[i*4+2] = hah ? 13 : 99; // (i*1.0f/size)*255 < 255 ? (i*1.0f/size)*255 : 255;
    data[i*4+3] = mr(); // hah ? 5 : 250;// (int)((i/float(size))*255); // mr(); 
    hah = !hah;
  }
//  for (int i=0 ; i<width*height*depth ; i++)
//  {
//    if (width*height*i 
//    data[i*4] = 65;// 0.5f; //   (i*1.0f/size)*255 < 255 ? (i*1.0f/size)*255 : 255;
//    data[i*4+1] = i % 255; // 1.0f; //(i*1.0f/size) < 1.0f ? (i*1.0f/size) : 1.0f;
//    data[i*4+2] = hah ? 13 : 99; // (i*1.0f/size)*255 < 255 ? (i*1.0f/size)*255 : 255;
//    data[i*4+3] = hah ? 0 : 255;// (int)((i/float(size))*255); // mr(); 
//    hah = !hah;
//  }
  return std::move(td);
}
