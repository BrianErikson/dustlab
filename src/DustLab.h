#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>

class DustLab {
 public:
  DustLab(bool headless = false);
  ~DustLab();
  bool init();
  int run();
  void handle_event(const SDL_Event &ev);
  void quit();

 private:
  bool init_gl();
  void render();
  void handle_window_event(const SDL_WindowEvent &ev);

  SDL_Window *window_{nullptr};
  SDL_GLContext gl_ctx_{nullptr};
  const bool headless_{false};
  bool running_{false};
};


