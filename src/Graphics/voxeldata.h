#ifndef VOXELDATA_H
#define VOXELDATA_H

#include <vector>
#include <string>
#include <tuple>
#include "../Utils/log.h"

//typedef struct Slice {
//  int x;  
//}
   

class VoxelData
{
	public:
		VoxelData();
		VoxelData(const int xSize, const int ySize, const int zSize);
		~VoxelData();

    float getValue(const int i, const int j, const int k) const;
    void setValue(const int i, const int j, const int k, float value) ;

    std::tuple<int,int,int> getDimensions() const;

	private:
    int pMaxI = 0;
    int pMaxJ = 0;
    int pMaxK = 0;
    std::vector<float> pData;
          
};


#endif // VOXELDATA_H
