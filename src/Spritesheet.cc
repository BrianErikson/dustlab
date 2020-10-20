#include <SDL2/SDL_log.h>
#include <vector>
#include <SDL2/SDL_video.h>
#include "Spritesheet.h"

Spritesheet::Spritesheet(const std::string &filepath, const Size &sprite_size) : Texture{filepath},
sprite_size{sprite_size}
{

}

bool Spritesheet::init() {
  if (!Texture::init()) {
    return false;
  }

  if (this->size_.width % sprite_size.width != 0 || this->size_.height % sprite_size.height != 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "invalid sprite_size passed to spritesheet");
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "w: %d, h: %d", this->size_.width, this->size_.height);
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%d:%d", this->size_.width % sprite_size.width, this->size_.height % sprite_size.height);
    return false;
  }

  this->subdiv_rows_ = this->size_.height / sprite_size.height;
  this->subdiv_cols_ = this->size_.width / sprite_size.width;

  const float offset = 1.f;
  float w_stride = this->sprite_size.width / (float)this->size_.width;
  float h_stride = this->sprite_size.height / (float)this->size_.height;
  float sw_stride = w_stride * 2; // scaled 2x to scale across -1 to 1 instead of 0 to 1
  float sh_stride = h_stride * 2;

  std::vector<GLfloat> buffer_attrs((this->subdiv_rows_ * this->subdiv_cols_) * (4 * 4));
  std::vector<std::vector<GLubyte>> buffer_idxs(this->subdiv_rows_ * this->subdiv_cols_);

  for (int y = 0; y < this->subdiv_rows_; y++) {
    std::vector<GLubyte> sprite_idxs{4};
    for (int x = 0; x < this->subdiv_cols_; x++) {
      buffer_attrs.emplace_back(x * sw_stride - offset); // blx
      buffer_attrs.emplace_back(y * sh_stride - offset); // bly
      buffer_attrs.emplace_back(x * w_stride); // blu
      buffer_attrs.emplace_back(1 - (y * w_stride)); // blv

      buffer_attrs.emplace_back(x * sw_stride - offset + sw_stride); // brx
      buffer_attrs.emplace_back(y * sh_stride - offset); // bry
      buffer_attrs.emplace_back(x * w_stride); // bru
      buffer_attrs.emplace_back(1 - (y * w_stride)); // brv

      buffer_attrs.emplace_back(x * sw_stride - offset + sw_stride); // trx
      buffer_attrs.emplace_back(y * sh_stride - offset + sh_stride); // try
      buffer_attrs.emplace_back(x * w_stride); // tru
      buffer_attrs.emplace_back(1 - (y * w_stride)); // trv

      buffer_attrs.emplace_back(x * sw_stride - offset); // tlx
      buffer_attrs.emplace_back(y * sh_stride - offset + sh_stride); // tly
      buffer_attrs.emplace_back(x * w_stride); // blu
      buffer_attrs.emplace_back(1 - (y * w_stride)); // trv

      int idx_offset = buffer_idxs.size() * 4;
      sprite_idxs.emplace_back(3 + idx_offset);
      sprite_idxs.emplace_back(2 + idx_offset);
      sprite_idxs.emplace_back(0 + idx_offset);
      sprite_idxs.emplace_back(1 + idx_offset);
      buffer_idxs.emplace_back(sprite_idxs);
    }
  }

  glGenVertexArrays(1, &this->ss_vao_);
  glBindVertexArray(this->ss_vao_);

  glGenBuffers(1, &this->ss_vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, this->ss_vbo_);
  glBufferData(GL_ARRAY_BUFFER, buffer_attrs.size() * sizeof(GLfloat), buffer_attrs.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  GLenum gl_err = glGetError();
  if (gl_err != GL_NO_ERROR) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Could not create a VAO/VBO: %s", gluErrorString(gl_err));
    return false;
  }

  this->ss_ibos_size = buffer_idxs.size();
  this->ss_ibos = static_cast<GLuint*>(malloc(buffer_idxs.size() * sizeof(GLuint)));
  glGenBuffers(buffer_idxs.size(), this->ss_ibos);

  for (int i = 0; i < this->ss_ibos_size; i++) {
    GLuint &ibo = this->ss_ibos[i];
    std::vector<GLubyte> &sprite_idxs = buffer_idxs[i];
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sprite_idxs.size() * sizeof(GLubyte), sprite_idxs.data(),
                 GL_STATIC_DRAW);

    gl_err = glGetError();
    if (gl_err != GL_NO_ERROR) {
      SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Could not create a IBO: %s", gluErrorString(gl_err));
      return false;
    }
  }

  return true;
}

int Spritesheet::rows() const {
  return this->subdiv_rows_;
}

int Spritesheet::cols() const {
  return this->subdiv_cols_;
}

const unsigned char *Spritesheet::sprite(int row, int col) {
  if (row > this->subdiv_rows_ || col > this->subdiv_cols_ || !this->image_) {
    return nullptr;
  }

  return this->image_.get() + (this->size_.width * row + col);
}

void Spritesheet::bind(int row, int col) {
  const auto &sprite_ibo = this->ss_ibos[this->ss_ibos_size * row + col];
  glBindVertexArray(this->ss_vao_);
  glBindBuffer(GL_ARRAY_BUFFER, this->ss_vbo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite_ibo);
}
