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

//void logVec2(const std::string& vecName, const glm::vec2& vec)
//{
//  Log::getDebug().log("% = (%,%)",vecName,std::to_string(vec.x),std::to_string(vec.y));
//}
//
//void logVec3(const std::string& vecName, const glm::vec3& vec)
//{
//  Log::getDebug().log("% = (%,%,%)",vecName,std::to_string(vec.x),std::to_string(vec.y),std::to_string(vec.z));
//}
//
//void logVec4(const std::string& vecName, const glm::vec4& vec)
//{
//  Log::getDebug().log("% = (%,%,%,%)",vecName,std::to_string(vec.x),std::to_string(vec.y),std::to_string(vec.w));
//}

//void logMat3(const std::string& matrixName, const glm::mat3& mat)
//{
//  std::string padded = addPadding("",matrixName.size(),' ');
//  Log::getDebug().log("% = (%,%,%)",matrixName,std::to_string(mat[0][0]),std::to_string(mat[0][1]),std::to_string(mat[0][2]));
//  Log::getDebug().log("%   (%,%,%)",padded    ,std::to_string(mat[1][0]),std::to_string(mat[1][1]),std::to_string(mat[1][2]));
//  Log::getDebug().log("%   (%,%,%)",padded    ,std::to_string(mat[2][0]),std::to_string(mat[2][1]),std::to_string(mat[2][2]));
//}
//
//void logMat4(const std::string& matrixName, const glm::mat4& mat)
//{
//  std::string padded = addPadding("",matrixName.size(),' ');
//  Log::getDebug().log("% = (%,%,%,%)",matrixName,std::to_string(mat[0][0]),std::to_string(mat[0][1]),std::to_string(mat[0][2]),std::to_string(mat[0][3]));
//  Log::getDebug().log("%   (%,%,%,%)",padded    ,std::to_string(mat[1][0]),std::to_string(mat[1][1]),std::to_string(mat[1][2]),std::to_string(mat[1][3]));
//  Log::getDebug().log("%   (%,%,%,%)",padded    ,std::to_string(mat[2][0]),std::to_string(mat[2][1]),std::to_string(mat[2][2]),std::to_string(mat[2][3]));
//  Log::getDebug().log("%   (%,%,%,%)",padded    ,std::to_string(mat[3][0]),std::to_string(mat[3][1]),std::to_string(mat[3][2]),std::to_string(mat[3][3]));
//}
