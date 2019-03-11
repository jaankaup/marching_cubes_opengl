#ifndef CELL_H
#define CELL_H
#include <iostream>
#include <vector>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/* Data object. */
class Cell
{
	public:
    Cell();
		Cell(const std::array<glm::vec3,8>& positions, const std::array<float,8>& isovalues);
		~Cell();

    // The values for cell corners.
    std::array<float,8> pIsovalues;
    std::array<glm::vec3, 8> pPositions;
     
    int pIndex;
//    std::vector<glm::vec3> pTriangles; 
//    std::vector<glm::vec3> pNormals; 

    // The indices which indicates the location of the data object in the
    // dataset.
    glm::ivec3 pPosition;

	private:	

};

#endif // CELL_H
