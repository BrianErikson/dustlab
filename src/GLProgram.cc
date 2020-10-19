#include <SDL2/SDL_log.h>
#include <GL/glew.h>
#include "GLProgram.h"

GLProgram::GLProgram(const std::string &vert_path, const std::string &frag_path) : vert_path(vert_path),
frag_path(frag_path) {

}

GLProgram::~GLProgram() {
  glUseProgram(0);

  glDetachShader(this->id_, this->vertex_shader_->get_id());
  glDetachShader(this->id_, this->fragment_shader_->get_id());

  this->vertex_shader_.reset();
  this->fragment_shader_.reset();

  glDeleteProgram(this->id_);
}

bool GLProgram::init() {
  this->vertex_shader_ = std::make_unique<GLShader>(this->vert_path);
  this->fragment_shader_ = std::make_unique<GLShader>(this->frag_path);

  if (!this->vertex_shader_->init()) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create vertex shader for GLProgram");
    return false;
  }

  if (!this->fragment_shader_->init()) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create fragment shader for GLProgram");
    return false;
  }

  this->id_ = glCreateProgram();
  glAttachShader(this->id_, this->vertex_shader_->get_id());
  glAttachShader(this->id_, this->fragment_shader_->get_id());
  glLinkProgram(this->id_);
  glUseProgram(this->id_);

  GLenum gl_err = glGetError();
  if (gl_err != GL_NO_ERROR) {
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Could not create GLProgram: %s", gluErrorString(gl_err));
    return false;
  }

  return true;
}

void GLProgram::use() const {
  glUseProgram(this->id_);
}
