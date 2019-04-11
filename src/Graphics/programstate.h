#ifndef PROGRAMSATE_H
#define PROGRAMSATE_H
//#include <iostream>
//#include <vector>
//#include <array>
//#include <GL/glew.h>
//#include <glm/glm.hpp>
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


    void setScreenWidth(const int width);
    void setScreenHeight(const int height);
    void setAppRunning(const bool running);
    void setVoxelsPerBlock(const float count);
    void setWireframe(const bool wireframeEnabled);

	private:

		ProgramState();
    int pScreenWidth = 1200;
    int pScreenHeight = 800;
    bool pAppRunning = true;
    float pVoxels_per_count = 1.0;
    bool pWireframe = false;

};


#endif // PROGRAMSATE_H
