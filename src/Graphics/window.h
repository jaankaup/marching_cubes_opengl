#ifndef WINDOW_H
#define WINDOW_H

//#include <memory>
#include <SDL2/SDL.h>
#include <GL/glew.h>

struct SDL_Window;
using SDL_GLContext = void*;

/* A window class using SDL2 window. */
class Window
{
  public:

    // Creates a window and initializes SDL2.
    Window();

    // Destroys both window and SDL2.
    ~Window();

    // Initializes window.
    bool init(int width, int height);

    // Swaps buffers.
    void swapBuffers();

    // Resize method for window.
    void resize(int width, int height);

    // Remove?
    void isValid() const;

  private:

    SDL_Window* pWindow = NULL; 
    SDL_GLContext pContext = NULL; 

}; // Class window

#endif //WINDOW_H
