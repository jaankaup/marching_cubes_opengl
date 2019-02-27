#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emscripten.h>
#include <math.h>

#include "Graphics/window.h"
#include "Graphics/shader.h"
#include "Graphics/model.h"
#include "Graphics/vertexbuffer.h"
#include "Graphics/indexbuffer.h"
#include "Graphics/vertexAttributes.h"
#include "Graphics/renderer.h"
#include "Graphics/texture.h"
#include "Utils/log.h"
#include "Utils/kokeilu.h"

/**
 * Inverse square root of two, for normalising velocity
 */
#define REC_SQRT2 0.7071067811865475 

/**
 * Set of input states
 */
enum input_state : int
{
    NOTHING_PRESSED = 0,
    UP_PRESSED = 1,
    DOWN_PRESSED = 1<<1,
    LEFT_PRESSED = 1<<2,
    RIGHT_PRESSED = 1<<3
};

/**
 * Context structure that will be passed to the loop handler
 */
struct context
{
    Window window;
    Renderer renderer;
    Shader shader;
    Vertexbuffer vertexbuffer;
    Texture texture;
};

/**
 * Processes the input events and sets the velocity
 * of the owl accordingly
 */
void process_input()
{
    SDL_Event event;
//
    while (SDL_PollEvent(&event)) {
//        switch (event.key.keysym.sym)
//        {
//            case SDLK_UP:
//                if (event.key.type == SDL_KEYDOWN)
//                    ctx->active_state |= UP_PRESSED;
//                else if (event.key.type == SDL_KEYUP)
//                    ctx->active_state ^= UP_PRESSED;
//                break;
//            case SDLK_DOWN:
//                if (event.key.type == SDL_KEYDOWN)
//                    ctx->active_state |= DOWN_PRESSED;
//                else if (event.key.type == SDL_KEYUP)
//                    ctx->active_state ^= DOWN_PRESSED;
//                break;
//            case SDLK_LEFT:
//                if (event.key.type == SDL_KEYDOWN)
//                    ctx->active_state |= LEFT_PRESSED;
//                else if (event.key.type == SDL_KEYUP)
//                    ctx->active_state ^= LEFT_PRESSED;
//                break;
//            case SDLK_RIGHT:
//                if (event.key.type == SDL_KEYDOWN)
//                    ctx->active_state |= RIGHT_PRESSED;
//                else if (event.key.type == SDL_KEYUP)
//                    ctx->active_state ^= RIGHT_PRESSED;
//                break;
//            default:
//                break;
//        }
    }
//
//    ctx->owl_vy = 0;
//    ctx->owl_vx = 0;
//    if (ctx->active_state & UP_PRESSED)
//        ctx->owl_vy = -5;
//    if (ctx->active_state & DOWN_PRESSED)
//        ctx->owl_vy = 5;
//    if (ctx->active_state & LEFT_PRESSED)
//        ctx->owl_vx = -5;
//    if (ctx->active_state & RIGHT_PRESSED)
//        ctx->owl_vx = 5;
//
//    if (ctx->owl_vx != 0 && ctx->owl_vy != 0)
//    {
//        ctx->owl_vx *= REC_SQRT2;
//        ctx->owl_vy *= REC_SQRT2;
//    }
}

void loop_handler2(void *arg)
{
//  auto time = SDL_GetTicks() * 0.005f;
//  auto value = sin(time) * 20;
  process_input();
    context* c = static_cast<context*>(arg);
//    Window *ctx = static_cast<Window*>(arg);
//    c->texture.bind();
    c->renderer.render(c->vertexbuffer,c->shader);
    c->window.swapBuffers();

//    int vx = 0;
//    int vy = 0;
//    process_input(ctx);
//
//    ctx->dest.x += ctx->owl_vx;
//    ctx->dest.y += ctx->owl_vy;
//
//    SDL_RenderClear(ctx->renderer);
//    SDL_RenderCopy(ctx->renderer, ctx->owl_tex, NULL, &ctx->dest);
//    SDL_RenderPresent(ctx->renderer);
}

int main()
{
  Window w;
  context c;
  c.window = std::move(w); 
  c.window.init(800,800);
  Shader s;
  std::vector<std::string> shaderSources = {"shaders/default.vert", "shaders/default.frag"};
  Vertexbuffer vb;
//  vb.createExampleCube();
 // Indexbuffer ib;
 // Model m;
//  VertexAttributes atr;
  Renderer r;
//  int a = 0;

  c.renderer = r; 
  c.renderer.init();

  c.vertexbuffer = vb; 
  c.vertexbuffer.init();
  c.vertexbuffer.createExampleCube();

  c.shader = s; 
  c.shader.init();
  c.shader.build(shaderSources);
  
  Texture t;
  c.texture = t;
  c.texture.init();
  c.texture.create("assets/rock.jpg");

  //ShaderManager::getInstance().createShader(shaderSources2, "pah");
//    SDL_Window *window;
//    struct context ctx;

//    SDL_Init(SDL_INIT_VIDEO);
//    SDL_CreateWindowAndRenderer(600, 400, 0, &window, &ctx.renderer);
//    SDL_SetRenderDrawColor(ctx.renderer, 255, 255, 255, 255);
//
//    get_owl_texture(&ctx);
//    ctx.active_state = NOTHING_PRESSED;
//    ctx.dest.x = 200;
//    ctx.dest.y = 100;
//    ctx.owl_vx = 0;
//    ctx.owl_vy = 0;

    /**
     * Schedule the main loop handler to get 
     * called on each animation frame
     */
    emscripten_set_main_loop_arg(loop_handler2, &c, -1, 1);

    return 0;
}

