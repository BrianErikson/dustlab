#include <SDL2/SDL_log.h>
#include "Quad.h"

Quad::Quad(int id) : Renderable(id) {

}

Quad::~Quad() {
  glDisableVertexAttribArray(this->id);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &this->quad_vbo);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &this->quad_ibo);

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &this->quad_vao);

  GLenum gl_err = glGetError();
  if (gl_err != GL_NO_ERROR) {
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Could not destroy a VBO: %s", gluErrorString(gl_err));
  }
}

void Quad::bind() const {
  glBindVertexArray(this->quad_vao);
  glBindBuffer(GL_ARRAY_BUFFER, this->quad_vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->quad_ibo);
}

void Quad::render() {
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, nullptr);
}

bool Quad::init() {
  static const GLfloat quad_verts[] = {
      -1.f, -1.f, 0.f, 1.f, // bl
      1.f, -1.f, 1.f, 1.f, // br
      1.f, 1.f, 1.f, 0.f, // tr
      -1.f, 1.f, 0.f, 0.f // tl
  };
  static const GLubyte quad_idxs[] = {
      3, 2, 0, 1
  };

  glGenVertexArrays(1, &this->quad_vao);
  glBindVertexArray(this->quad_vao);

  glGenBuffers(1, &this->quad_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, this->quad_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad_verts), quad_verts, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &this->quad_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->quad_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_idxs), quad_idxs, GL_STATIC_DRAW);

  GLenum gl_err = glGetError();
  if (gl_err != GL_NO_ERROR) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Could not create a VBO: %s", gluErrorString(gl_err));
    return false;
  }

  return true;
}
