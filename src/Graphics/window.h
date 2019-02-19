#ifndef WINDOW_H
#define WINDOW_H

//#include <memory>
#include <SDL2/SDL.h>
#include <GL/glew.h>

struct SDL_Window;
using SDL_GLContext = void*;

/* Ikkuna luokka. */
class Window
{
  public:
    Window();
    ~Window();

    bool init(int width, int height);
    void swapBuffers();
    void resize(int width, int height);
    void isValid() const;

  private:

    SDL_Window* pWindow = NULL; 
    SDL_GLContext pContext = NULL; 
    bool pBlah = false;

}; // Class window

#endif //WINDOW_H
