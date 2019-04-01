#include "programstate.h"

ProgramState::ProgramState()
{

}

ProgramState::~ProgramState()
{

}

ProgramState& ProgramState::getInstance()
{
    static ProgramState instance;
    return instance;
}

int ProgramState::getScreenWidth() const
{
    return pScreenWidth;

}
int ProgramState::getScreenHeight() const
{
    return pScreenHeight;
}

bool ProgramState::getAppRunning() const
{
    return pAppRunning;
}

void ProgramState::setScreenWidth(const int width)
{
  if (width > 0) pScreenWidth = width;

}

void ProgramState::setScreenHeight(const int height)
{
  if (height > 0) pScreenHeight = height;
}

void ProgramState::setAppRunning(const bool running)
{
  pAppRunning = running;
}

