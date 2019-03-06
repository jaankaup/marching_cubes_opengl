#include "cell.h"

Cell::Cell() {}

Cell::Cell(const std::array<glm::ivec3, 8>& positions, const std::array<float,8>& isovalues)
{
  pPositions = std::move(positions);
  pIsovalues = std::move(isovalues);
}

Cell::~Cell()
{
}
