#ifndef MISC_H
#define MISC_H

#include <iostream>
#include <cmath>
#include <algorithm>
//#include <type_traits>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "log.h"
#include "myrandom.h"
#include "../Graphics/textureData.h"

extern void joopajoo();
extern std::string addPadding(const std::string& str, const uint32_t count, const char paddingChar);
extern void logMatrix(const std::string& matrixName, const glm::mat4& mat);

extern void logGLM(const std::string& name, const glm::vec2& object);
extern void logGLM(const std::string& name, const glm::vec3& object);
extern void logGLM(const std::string& name, const glm::vec4& object);
extern void logGLM(const std::string& name, const glm::mat3& object);
extern void logGLM(const std::string& name, const glm::mat4& object);

extern TextureData createRandom3Ddata(const int width, const int height, const int depth);
extern TextureData createChess3Ddata(const  int width, const int height, const int depth);

extern TextureData create2x2();

extern double centerY(int y);


#endif //MISC_H
