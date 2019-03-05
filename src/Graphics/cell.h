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
		~Cell();

	private:	
    // The values for cell corners.
    std::array<float,8> pVolumeData;
    // The indices which indicates the location of the data object in the
    // dataset.
    glm::ivec3 pPosition;
};

#endif // CELL_H
