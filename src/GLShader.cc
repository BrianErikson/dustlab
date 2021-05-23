#include <GL/glew.h>
#include <string>
#include <fstream>
#include <SDL2/SDL_log.h>
#include "GLShader.h"

GLShader::GLShader(const std::string &filepath) : filepath(filepath) {
  if (this->filepath.rfind(".vert") != std::string::npos) {
    this->shader_type = GL_VERTEX_SHADER;
  }
  else if (this->filepath.rfind(".frag") != std::string::npos) {
    this->shader_type = GL_FRAGMENT_SHADER;
  }
}

GLShader::~GLShader() {
  glDeleteShader(this->id_);

  GLenum gl_err = glGetError();
  if (gl_err != GL_NO_ERROR) {
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Could not destroy shader: %s\n %s", this->filepath.c_str(),
                gluErrorString(gl_err));
  }
}

bool GLShader::init() {
  if (!this->shader_type) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unknown shader type for file: %s", this->filepath.c_str());
    return false;
  }

  this->id_ = glCreateShader(this->shader_type);
  std::ifstream is{this->filepath};
  if (!is.is_open()) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not open file: %s", this->filepath.c_str());
    return false;
  }

  std::string file{std::istreambuf_iterator<char>{is}, std::istreambuf_iterator<char>{}};
  auto *text = const_cast<GLchar*>(file.c_str());
  glShaderSource(this->id_, 1, &text, nullptr);
  glCompileShader(this->id_);

  GLenum gl_err = glGetError();
  if (gl_err != GL_NO_ERROR) {
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Could not create shader: %s\n %s", this->filepath.c_str(),
                gluErrorString(gl_err));
    return false;
  }

  return true;
}

GLuint GLShader::get_id() {
  return this->id_;
}
