#ifndef PROGRAMSATE_H
#define PROGRAMSATE_H
//#include <iostream>
//#include <vector>
//#include <array>
//#include <GL/glew.h>
#include <cmath>
#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include "../Utils/log.h"

class ProgramState
{

	public:

    static ProgramState& getInstance();
		~ProgramState();

    int getScreenWidth() const;
    int getScreenHeight() const;
    bool getAppRunning() const;
    float getVoxelsPerBlock() const;
    bool getWireframe() const;
    glm::vec3 getStartPoint() const;
    float getCubeMask() const;
    float getCubeMaskCeil() const;
    bool getDebugCube() const;
    glm::ivec3 getCubeDimension() const;


    void setScreenWidth(const int width);
    void setScreenHeight(const int height);
    void setAppRunning(const bool running);
    void setVoxelsPerBlock(const float count);
    void setWireframe(const bool wireframeEnabled);
    void setStartPoint(const glm::vec3& point);
    void setCubeMask(const float cubeMask);
    void setDebugCube(const bool debugCube);
    void setCubeDimension(const glm::ivec3& cubeDimension);

	private:

		ProgramState();
    int pScreenWidth = 1200;
    int pScreenHeight = 800;
    bool pAppRunning = true;
    float pVoxels_per_count = 1.0;
    bool pWireframe = false;
    glm::vec3 pStartPoint = glm::vec3(0.0,0.0,0.0);
    float pCubeMask = 0.1f;
    bool pDebugCube = false;
    glm::ivec3 pCubeDimension;

};


#endif // PROGRAMSATE_H
