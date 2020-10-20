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
  /*
  this->uloc_model_ = glGetUniformLocation(this->id_, "model");
  this->uloc_projection_ = glGetUniformLocation(this->id_, "projection");

  GLenum gl_err = glGetError();
  if (gl_err != GL_NO_ERROR || this->uloc_model_ < 0 || this->uloc_projection_ < 0 ||
      this->uloc_sprite_color_ < 0) {

    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create get uniform locations for default program");
    return false;
  }
   */

  return true;
}

void GLDefaultProgram::use(const glm::mat4 &model, const glm::mat4 &projection, const glm::vec3 &color) {
  GLProgram::use();
  glProgramUniform3fv(this->id_, this->uloc_sprite_color_, 1, glm::value_ptr(color));
  /*
  glProgramUniformMatrix4fv(this->id_, this->uloc_model_, 1, GL_FALSE, glm::value_ptr(model));
  glProgramUniformMatrix4fv(this->id_, this->uloc_projection_, 1, GL_FALSE, glm::value_ptr(projection));
   */
}
