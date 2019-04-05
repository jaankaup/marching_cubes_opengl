#include "textureData.h"

TextureData::TextureData(const uint32_t dataSize, const int width, const int height, const int depth)
{
  if (! (setWidth(width) && setHeight(height) && setDepth(depth)) )
  {
    Log::getError().log("TextureData::TextureData(data,%,%,%,%).",std::to_string(dataSize),std::to_string(width),std::to_string(height),std::to_string(depth));
    Log::getError().log("TextureData::TextureData: the state of TextureData object is undefined. ",std::to_string(dataSize),std::to_string(width),std::to_string(height),std::to_string(depth));
  } 
  else
  {
    pData = std::make_unique<unsigned char[]>(dataSize);   
  }
}

TextureData::~TextureData()
{

}

//const std::vector<unsigned char>& TextureData::getData() const
unsigned char* TextureData::getData() const
{
  return pData.get();
}


bool TextureData::setWidth(const int w)
{
  int result;
  Log::getDebug().log("TextureData::setWidth: result = %.", std::to_string(result));
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &result); 
  if (w > result)
  {
    Log::getError().log("TextureData::setWidth(%): GL_MAX_TEXTURE_SIZE == %.", std::to_string(w),std::to_string(result));
    return false;
  }
  pWidth = w;
  return true;
}

bool TextureData::setHeight(const int h)
{

  int result;
  Log::getDebug().log("TextureData::setHeight: result = %.", std::to_string(result));
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &result); 
  if (h > result)
  {
    Log::getError().log("TextureData::setHeight(%): GL_MAX_TEXTURE_SIZE == %.", std::to_string(h),std::to_string(result));
    return false;
  }
  pHeight = h;
  return true;
}

bool TextureData::setDepth(const int d)
{
  int result;
  Log::getDebug().log("TextureData::setDepth: result = %.", std::to_string(result));
  glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &result); 
  if (d > result)
  {
    Log::getError().log("TextureData::setDepth(%): GL_MAX_TEXTURE_SIZE == %.", std::to_string(d),std::to_string(result));
    return false;
  }
  pDepth = d;
  return true;
}

int TextureData::getWidth() const
{
  return pWidth;
}

int TextureData::getHeight() const
{
  return pHeight;
}

int TextureData::getDepth() const
{
  return pDepth;
}
