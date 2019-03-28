#ifndef MISC_H
#define MISC_H

#include <iostream>
//#include <type_traits>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "log.h"

extern void joopajoo();
extern std::string addPadding(const std::string& str, const uint32_t count, const char paddingChar);
extern void logMatrix(const std::string& matrixName, const glm::mat4& mat);

extern void logGLM(const std::string& name, const glm::vec2& object);
extern void logGLM(const std::string& name, const glm::vec3& object);
extern void logGLM(const std::string& name, const glm::vec4& object);
extern void logGLM(const std::string& name, const glm::mat3& object);
extern void logGLM(const std::string& name, const glm::mat4& object);

#endif //MISC_H
