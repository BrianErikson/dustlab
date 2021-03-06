#include <iostream>
#include <chrono>
#include <timer-wheel.h>
#include <ecs/EntityGenerator.h>
#include <glm/ext.hpp>
#include "DustLab.h"
#include "GLProgram.h"
#include "geometry/Quad.h"
#include "SpriteRenderer.h"
#include "Spritesheet.h"
#include "Transform.h"
#include "ecs/core.h"
#include "WorldGenerator.h"

void printProgramLog(GLuint program) {
  //Make sure name is shader
  if (glIsProgram(program)) {
    //Program log length
    int infoLogLength = 0;
    int maxLength = infoLogLength;

    //Get info string length
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    //Allocate string
    char *infoLog = new char[maxLength];

    //Get info log
    glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
    if (infoLogLength > 0) {
      //Print Log
      printf("%s\n", infoLog);
    }

    //Deallocate string
    delete[] infoLog;
  } else {
    printf("Name %d is not a program\n", program);
  }
}

void printShaderLog(GLuint shader) {
  //Make sure name is shader
  if (glIsShader(shader)) {
    //Shader log length
    int infoLogLength = 0;
    int maxLength = infoLogLength;

    //Get info string length
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    //Allocate string
    char *infoLog = new char[maxLength];

    //Get info log
    glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
    if (infoLogLength > 0) {
      //Print Log
      printf("%s\n", infoLog);
    }

    //Deallocate string
    delete[] infoLog;
  } else {
    printf("Name %d is not a shader\n", shader);
  }
}

DustLab::DustLab(bool headless) :
    headless_(headless) {
}

DustLab::~DustLab() {
  if (this->gl_ctx_) {
    SDL_GL_DeleteContext(this->gl_ctx_);
  }
  if (this->window_) {
    SDL_DestroyWindow(window_);
  }
  SDL_Quit();
}

bool DustLab::init() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialze SDL: %s", SDL_GetError());
    return false;
  }

  this->window_ = SDL_CreateWindow("DustLab", 0, 0, 1920, 1440, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  if (!this->window_) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
    return false;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  this->gl_ctx_ = SDL_GL_CreateContext(this->window_);
  if (!this->gl_ctx_) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Couldn't create OpenGL %d.%d context: %s",
                 3,
                 1,
                 SDL_GetError());
    return false;
  }

  glewExperimental = true;
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing glew: %s", SDL_GetError());
    return false;
  }

  // use vsync for now
  SDL_GL_SetSwapInterval(1);

  if (!this->init_gl()) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error initializing OpenGL program");
    return false;
  }

  return true;
}

int DustLab::run() {
  this->running_ = true;

  SpriteRenderer renderer{};
  if (!renderer.init()) {
    return 2;
  }

  EntityGenerator generator;
  auto opt = generator.BasicBot();
  if (!opt.has_value()) {
    return 2;
  }
  auto ebot = opt.value();

  Scene scene;
  scene.tilesheet = std::make_shared<Tilesheet>("./res/textures/desert_tileset.png", "./res/textures/desert_tileset.json");
  scene.tilesheet->init();
  scene.map = WorldGenerator::get_map();

  SDL_Event ev;

  auto now = std::chrono::high_resolution_clock::now();
  while (this->running_) {
    SDL_PollEvent(&ev);
    this->handle_event(ev);

    auto dt_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - now).count();
    if (dt_ms > 0) {
      this->registry_.update(dt_ms);
    }
    now = std::chrono::high_resolution_clock::now();

    SDL_GL_SwapWindow(this->window_);
    glClearColor(0.0f, 0.f, 0.f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    renderer.render(scene, glm::mat4{1.f}, glm::vec3{1.f});
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    renderer.render(ebot);
    glDisable(GL_BLEND);
  }

  return 0;
}

void DustLab::handle_event(const SDL_Event &ev) {
  switch (ev.type) {
    case SDL_FIRSTEVENT:
      // No-op
      break;
    case SDL_QUIT:this->quit();
      break;
    case SDL_WINDOWEVENT:
      this->handle_window_event((const SDL_WindowEvent &) ev);
      break;
    default:
      break;
  }
}

void DustLab::handle_window_event(const SDL_WindowEvent &ev) {
  switch (ev.event) {
    case SDL_WINDOWEVENT_SIZE_CHANGED:
    case SDL_WINDOWEVENT_RESIZED:
      SDL_Log("Window %d resized to %dx%d", ev.windowID, ev.data1, ev.data2);
      this->registry_.view().set_scale({this->registry_.zoom * ev.data1, this->registry_.zoom * ev.data2});
      this->registry_.projection().set(glm::ortho(0.f, (float)ev.data1, 0.f, (float)ev.data2, -1.0f, 1.f));
      break;
    default:
      break;
  }
}

void DustLab::render() {
  SDL_GL_SwapWindow(this->window_);
  glClearColor(1.0f, 0.f, 1.f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  /*
  //Clear color buffer
  glClear( GL_COLOR_BUFFER_BIT );

  //Bind program
  glUseProgram( gProgramID );

  //Enable vertex position
  glEnableVertexAttribArray( gVertexPos2DLocation );

  //Set vertex data
  glBindBuffer( GL_ARRAY_BUFFER, gVBO );
  glVertexAttribPointer( gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL );

  //Set index data and render
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
  glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );

  //Disable vertex position
  glDisableVertexAttribArray( gVertexPos2DLocation );

  //Unbind program
  glUseProgram(0);
   */
}

void DustLab::quit() {
  this->running_ = false;
}

bool DustLab::init_gl() {
  // TODO
  glViewport(0, 0, 1920, 1440);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, GL_TRUE);


  return true;
}
