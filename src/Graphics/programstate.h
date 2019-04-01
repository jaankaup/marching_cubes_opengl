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


    void setScreenWidth(const int width);
    void setScreenHeight(const int height);
    void setAppRunning(const bool running);

	private:

		ProgramState();
    int pScreenWidth = 1200;
    int pScreenHeight = 800;
    bool pAppRunning = true;

};


#endif // PROGRAMSATE_H
