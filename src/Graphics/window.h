#ifndef WINDOW_H
#define WINDOW_H

//#include <memory>
//#include <memory>
#include <SDL2/SDL.h>
#include <GL/glew.h>

struct SDL_Window;
using SDL_GLContext = void*;

/* A window class using SDL2 window. */
class Window
{

//  friend class WindowManager;

  public:

    static Window& getInstance();
//    Window& operator=(const Window& other) = delete;
//    Window& operator=(Window&& other);


    // Swaps buffers.
    void swapBuffers();

    // Resize method for window.
    void resize(int width, int height);

    // Remove?
    void isValid() const;

    // Doesn't destroy the SDL2 and window. Call dispose to do this.
    ~Window();

  private:

    SDL_Window* pWindow = NULL; 
    SDL_GLContext pContext = NULL; 

    //bool pBlah = false;

    // Creates a window and initializes SDL2.
    Window();

    // Initializes window.
    bool init(int width, int height);

    // Disposes the window. Destroys both window and SDL2.
    void dispose();


}; // Class window

#endif //WINDOW_H
