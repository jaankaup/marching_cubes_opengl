#include "testData.h"

/* A simple 16*16*16 data set. */
//std::array<Cell,4096> exampleVoxelDataset1()
std::vector<Cell> exampleVoxelDataset1()
{
  Log::getDebug().log("exampleVoxelDataset1()");
//  Cell result[4096];
  std::vector<Cell> result;
  result.resize(4096);
  for (int i=0 ; i<16 ; i++) {
    for (int j=0 ; j<16 ; j++) {
      for (int k=0 ; k<16 ; k++) {
        std::array<glm::ivec3,8> positions = {{
          {{glm::ivec3(i  ,j   ,k)}},       
          {{glm::ivec3(i+1,j   ,k)}},       
          {{glm::ivec3(i+1,j   ,k+1)}},       
          {{glm::ivec3(i  ,j   ,k+1)}},       
          {{glm::ivec3(i  ,j+1 ,k)}},       
          {{glm::ivec3(i+1,j+1 ,k)}},       
          {{glm::ivec3(i+1,j+1 ,k+1)}},       
          {{glm::ivec3(i  ,j+1 ,k+1)}}
        }};
//        glm::ivec3 pos(i,j,k);
        std::array<float,8> isovalues = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}; 
        Cell c(positions,isovalues); // Huonoa koodia. Ehka kopiointia.
      // result.push_back(c);
        result[i+16*j+16*16+k] = c;
      }
    }
  }
  result[40].pIsovalues = {0.5f,0.5f,0.5f,0.5f,0.5f,0.5f, 0.5f, 0.5f};
  result[1140].pIsovalues = {0.5f,0.5f,0.5f,0.5f,0.5f,0.5f, 0.5f, 0.5f};
  result[2140].pIsovalues = {0.5f,0.5f,0.5f,0.5f,0.5f,0.5f, 0.5f, 0.5f};
  return result;
}
