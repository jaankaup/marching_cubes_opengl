#ifndef TESTDATA_H
#define TESTDATA_H
#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cell.h"
#include "voxeldata.h"
#include "../Utils/log.h"

extern VoxelData exampleData1();
extern VoxelData exampleData2();
extern std::vector<glm::vec3> createSomething();

extern std::vector<Cell> exampleVoxelDataset1();

#endif // TESTDATA_H
