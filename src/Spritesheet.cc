#include <SDL2/SDL_log.h>
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
    return false;
  }

  this->subdiv_rows_ = this->size_.height / sprite_size.height;
  this->subdiv_cols_ = this->size_.width / sprite_size.width;

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
