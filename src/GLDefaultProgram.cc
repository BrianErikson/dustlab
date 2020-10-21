#include <SDL2/SDL_log.h>
#include <glm/gtc/type_ptr.hpp>
#include "GLDefaultProgram.h"

GLDefaultProgram::GLDefaultProgram() : GLProgram("./res/shaders/default.vert", "./res/shaders/default.frag") {

}

bool GLDefaultProgram::init() {
  if (!GLProgram::init()) {
    return false;
  }

  this->uloc_sprite_color_ = glGetUniformLocation(this->id_, "sprite_color");
  this->uloc_mvp_ = glGetUniformLocation(this->id_, "mvp");

  GLenum gl_err = glGetError();
  if (gl_err != GL_NO_ERROR || this->uloc_mvp_ < 0 || this->uloc_sprite_color_ < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create get uniform locations for default program");
    return false;
  }

  return true;
}

void GLDefaultProgram::use(const glm::mat4 &mvp, const glm::vec3 &color) {
  GLProgram::use();
  glProgramUniform3fv(this->id_, this->uloc_sprite_color_, 1, glm::value_ptr(color));
  glProgramUniformMatrix4fv(this->id_, this->uloc_mvp_, 1, GL_FALSE, glm::value_ptr(mvp));
}
