#include <GL/glew.h>
#include <SDL2/SDL_log.h>
#include <string>
#include "Texture.h"

Texture::Texture(const std::string &filepath) : filepath(filepath) {

}

bool Texture::init() {
  int width, height;
  this->image_.reset(SOIL_load_image(this->filepath.c_str(), &width, &height, nullptr, SOIL_LOAD_RGBA));
  if (!this->image_) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load texture: %s", this->filepath.c_str());
    return false;
  }

  glGenTextures(1, &this->id_);
  glBindTexture(GL_TEXTURE_2D, this->id_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->image_.get());
  // TODO: Do we need?
  glGenerateMipmap(GL_TEXTURE_2D);

  this->size_.width = width;
  this->size_.height = height;

  return true;
}

void Texture::bind() const {
  glBindTexture(GL_TEXTURE_2D, this->id_);
}

std::shared_ptr<const unsigned char> Texture::data() {
  return this->image_;
}

Size& Texture::size() {
  return this->size_;
}
