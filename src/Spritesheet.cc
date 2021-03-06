#include <SDL2/SDL_log.h>
#include <vector>
#include <iostream>
#include <glm/gtx/transform.hpp>
#include "Spritesheet.h"

Spritesheet::Spritesheet(const std::string &filepath, const Size<int> &sprite_size) : Texture{filepath},
                                                                                 sprite_size_{sprite_size}
{
}

Spritesheet::~Spritesheet() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &this->ss_vbo_);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &this->ss_ebo);

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &this->ss_vao_);
}

void Spritesheet::set_sprite_size(Size<int> sprite_size) {
  this->sprite_size_ = sprite_size;
}

bool Spritesheet::init() {
  if (this->ss_vao_) {
    return true;
  }

  if (!Texture::init()) {
    return false;
  }

  if (this->size_.width % sprite_size_.width != 0 || this->size_.height % sprite_size_.height != 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "invalid sprite_size passed to spritesheet");
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "w: %d, h: %d", this->size_.width, this->size_.height);
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%d:%d", this->size_.width % sprite_size_.width, this->size_.height % sprite_size_.height);
    return false;
  }

  this->subdiv_rows_ = this->size_.height / sprite_size_.height;
  this->subdiv_cols_ = this->size_.width / sprite_size_.width;

  this->stride_width_ = this->sprite_size_.width / (float)this->size_.width;
  this->stride_height_ = this->sprite_size_.height / (float)this->size_.height;

  std::vector<GLfloat> buffer_attrs{};
  std::vector<GLubyte> sprite_idxs{};
  int quad_count = 0;

  for (int y = 0; y < this->subdiv_rows_; y++) {
    for (int x = 0; x < this->subdiv_cols_; x++) {
      // uv is flipped on the x-axis on purpose
      buffer_attrs.emplace_back(x * stride_width_); // tlx
      buffer_attrs.emplace_back(y * stride_height_ + stride_height_); // tly
      buffer_attrs.emplace_back(x * this->stride_width_); // blu
      buffer_attrs.emplace_back(y * this->stride_height_); // blv

      buffer_attrs.emplace_back(x * stride_width_); // blx
      buffer_attrs.emplace_back(y * stride_height_); // bly
      buffer_attrs.emplace_back(x * this->stride_width_); // tlu
      buffer_attrs.emplace_back(y * this->stride_height_ + this->stride_height_); // tlv

      buffer_attrs.emplace_back(x * stride_width_ + stride_width_); // trx
      buffer_attrs.emplace_back(y * stride_height_ + stride_height_); // try
      buffer_attrs.emplace_back(x * this->stride_width_ + this->stride_width_); // bru
      buffer_attrs.emplace_back(y * this->stride_height_); // brv

      buffer_attrs.emplace_back(x * stride_width_ + stride_width_); // brx
      buffer_attrs.emplace_back(y * stride_height_); // bry
      buffer_attrs.emplace_back(x * this->stride_width_ + this->stride_width_); // tru
      buffer_attrs.emplace_back(y * this->stride_height_ + this->stride_height_); // trv

      int idx_offset = quad_count * 4;
      sprite_idxs.emplace_back(0 + idx_offset);
      sprite_idxs.emplace_back(1 + idx_offset);
      sprite_idxs.emplace_back(2 + idx_offset);
      sprite_idxs.emplace_back(3 + idx_offset);
      quad_count++;
    }
  }

  glGenVertexArrays(1, &this->ss_vao_);
  glBindVertexArray(this->ss_vao_);

  glGenBuffers(1, &this->ss_vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, this->ss_vbo_);
  glBufferData(GL_ARRAY_BUFFER, buffer_attrs.size() * sizeof(GLfloat), buffer_attrs.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  GLenum gl_err = glGetError();
  if (gl_err != GL_NO_ERROR) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Could not create a VAO/VBO: %s", gluErrorString(gl_err));
    return false;
  }

  glGenBuffers(1, &this->ss_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ss_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sprite_idxs.size() * sizeof(GLubyte), sprite_idxs.data(),
               GL_STATIC_DRAW);

  gl_err = glGetError();
  if (gl_err != GL_NO_ERROR) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Could not create a EBO: %s", gluErrorString(gl_err));
    return false;
  }

  return true;
}

int Spritesheet::rows() const {
  return this->subdiv_rows_;
}

int Spritesheet::cols() const {
  return this->subdiv_cols_;
}

glm::vec2 Spritesheet::image_offset(int row, int col) const {
  return {col * this->sprite_size_.width, row * this->sprite_size_.height};
}

glm::vec3 Spritesheet::model_offset(int row, int col) const {
  return glm::vec3{(float)col * this->stride_width_, (float)row * this->stride_height_, 0.f};
}

Size<float> Spritesheet::cell_size() const {
  return {this->stride_width_, this->stride_height_};
}

void Spritesheet::bind() const {
  Texture::bind();
  glBindVertexArray(this->ss_vao_);
  glBindBuffer(GL_ARRAY_BUFFER, this->ss_vbo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ss_ebo);
}
