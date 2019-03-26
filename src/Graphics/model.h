#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Command
{
  GLuint vao;
  std::string textureName;
  std::string shaderName;
  uint32_t startIndex = 0;
  uint32_t count = 0;
};

class Model
{

	public:
		Model();
		~Model();

    void addCommand(const Command& command);
    void addModelMatrix(const glm::mat4& modelMatrix);

	private:
    std::vector<Command> pCommands;
    glm::mat4 pModelMatrix;
};

#endif // MODEL_H
