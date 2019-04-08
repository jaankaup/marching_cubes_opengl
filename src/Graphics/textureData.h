#ifndef TEXTUREDATA_H
#define TEXTUREDATA_H

#include <vector>
#include <memory>
#include <utility>
#include <GL/glew.h>
#include "../Utils/log.h"

class TextureData
{

	public:

		TextureData(const int dataSize, const int width, const int height, const int depth);
		~TextureData();

    TextureData& operator=(TextureData&& other)
    {
      pData = std::move(other.pData);
      pWidth = other.pWidth;
      pHeight = other.pHeight;
      pDepth = other.pDepth;
      pSize = other.pSize;
      return *this;
    }

    TextureData(TextureData&& o) : pData(std::move(o.pData)),
                                   pWidth(std::exchange(o.pWidth,0)),
                                   pHeight(std::exchange(o.pHeight,0)),
                                   pDepth(std::exchange(o.pDepth,0)), 
                                   pSize(std::exchange(o.pSize,0)) {}

    unsigned char* getData() const;
//    const std::vector<unsigned char>& getData() const;
//    const std::vector<char>& getData() const;
//    std::vector<char>& getData();

//    bool setData(const char* data, const uint32_t dataSize, const int width, const int height, const int depth);

    int getWidth() const;
    int getHeight() const;
    int getDepth() const;
    int getDataSize() const;
    
	private:
    int pWidth = 0;
    int pHeight = 0;
    int pDepth = 0;
    int pSize = 0;
    std::unique_ptr<unsigned char[]> pData; 

    bool setWidth(const int w);
    bool setHeight(const int w);
    bool setDepth(const int w);
};

#endif // TEXTUREDATA_H
