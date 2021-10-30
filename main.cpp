// This program is a flyby around the RGB color cube.  One intersting note
// is that because the cube is a convex polyhedron and it is the only thing
// in the scene, we can render it using backface culling only. i.e., there
// is no need for a depth buffer.

#include <SDL2/SDL.h>
#include "window.h"
#include "CubeTextured.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

std::unique_ptr<Window> window;
static bool main_loop_running = true;
std::unique_ptr<Cube> cube;


// Display and Animation. To draw we just clear the window and draw the cube.
// Because our main window is double buffered we have to swap the buffers to
// make the drawing visible. Animation is achieved by successively moving our
// camera and drawing. The function nextAnimationFrame() moves the camera to
// the next point and draws. The way that we get animation in OpenGL is to
// register nextFrame as the idle function; this is done in main().
void display() {
    cube->display();
}

void main_loop() {
    SDL_Event event;
    static float t = 0.0;
    static bool buttonDown = false;
    double originX = 0.0;
    double originY = 0.0;
    static float mtx = 0.0;
    static float mty = 0.0;


    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                main_loop_running = false;
                break;
            }
            case SDL_KEYUP: {
                int sym = event.key.keysym.sym;
                if (sym == SDLK_ESCAPE) { main_loop_running = false; }
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                buttonDown = true;
                originX = event.button.x;
                originY = event.button.y;
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                buttonDown = false;
                break;
            }
            case SDL_MOUSEMOTION: {
                if (buttonDown) {
                    mtx -= event.motion.xrel * 0.5;
                    mty += event.motion.yrel * 0.5;
                }

            }
        }
        window->ProcessEvent(&event);
    }

    if (window->visible) {
        if (!buttonDown) {
            t += 2;
        }
        cube->move(t, mtx, mty);
        window->Render(display);
    }
}

void init() {
    window = std::unique_ptr<Window>(new Window(800, 800));

    cube = std::unique_ptr<Cube>(new CubeTextured());
    cube->init();
}
void destroy() {
    cube->destroy();
    cube = nullptr;
    window = nullptr;
}

// The usual main for a GLUT application.
int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { printf("error: SDL_Init"); }
    SDL_GL_SetSwapInterval(1);

    init();
#ifdef __EMSCRIPTEN__
    // 0 fps means to use requestAnimationFrame; non-0 means to use setTimeout.
  emscripten_set_main_loop(main_loop, 18, 1);
#else
    while (main_loop_running) {
        main_loop();
        SDL_Delay(16);
    }
#endif
    destroy();

    SDL_Quit();
}