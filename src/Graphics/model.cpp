#include "model.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::addCommand(const Command& command)
{
  pCommands.push_back(command);
}

void Model::addModelMatrix(const glm::mat4& modelMatrix)
{
  pModelMatrix = modelMatrix;
}
