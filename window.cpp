// Copyright 2015 Red Blob Games <redblobgames@gmail.com>
// License: Apache v2.0 <http://www.apache.org/licenses/LICENSE-2.0.html>

#include "window.h"

#include <SDL2/SDL.h>
//#include "glwrappers.h"

//#include <vector>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#endif


// NOTE: I need this implementation to be in a cpp file and not an h
// file so that the compiler knows where to put the vtable
//IRenderLayer::~IRenderLayer() {}
struct nocopy {
    nocopy() = default;
    nocopy(const nocopy&) = delete;
    nocopy& operator = (const nocopy&) = delete;
};
struct GlContext: nocopy {
    SDL_GLContext id;
    GlContext(SDL_Window* window);
    ~GlContext();
};
GlContext::GlContext(SDL_Window* window) {
    id = SDL_GL_CreateContext(window);
    if (id == nullptr) { printf("error: SDL_GL_CreateContext"); }
}

GlContext::~GlContext() {
    SDL_GL_DeleteContext(id);
}
struct WindowImpl {
  SDL_Window* window;
  bool context_initialized;
  GlContext context;
  
  WindowImpl(SDL_Window* window_);
  ~WindowImpl();
};

int Window::FRAME = 0;

Window::Window(int width_, int height_)
  :visible(true), width(width_), height(height_),
   self(new WindowImpl(SDL_CreateWindow(nullptr,
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            width,
                            height,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
                           )))
{
  HandleResize();
}

Window::~Window() {}


void Window::HandleResize() {
  self->context_initialized = false;
  SDL_GL_GetDrawableSize(self->window, &width, &height);
  if(width>height) {
      glViewport((width - height) / 2,0, height, height);
  } else {
      glViewport(0, (height - width) / 2, width, width);
  }
}


void Window::Render(void (*drawfn)()) {
  if (visible) {
    glClear(GL_COLOR_BUFFER_BIT);
    drawfn();
    self->context_initialized = true;
    SDL_GL_SwapWindow(self->window);
    FRAME++;
  }
}


void Window::ProcessEvent(SDL_Event* event) {
  if (event->type == SDL_WINDOWEVENT) {
    switch (event->window.event) {
    case SDL_WINDOWEVENT_SHOWN: { visible = true; break; }
    case SDL_WINDOWEVENT_HIDDEN: { visible = false; break; }
    case SDL_WINDOWEVENT_SIZE_CHANGED: { HandleResize(); break; }
    }
  }
}


WindowImpl::WindowImpl(SDL_Window* window_)
  :window(window_), context_initialized(false), context(window)
{
  //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  //glClearColor(1.0, 1.0, 1.0, 1.0);
}

WindowImpl::~WindowImpl() {
  SDL_DestroyWindow(window);
}
