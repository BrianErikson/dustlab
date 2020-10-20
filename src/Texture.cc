#include <SOIL.h>
#include <GL/glew.h>
#include <SDL2/SDL_log.h>
#include "Texture.h"

Texture::Texture(const std::string &filepath) : filepath(filepath) {

}

bool Texture::init() {
  int width, height;
  unsigned char *image = SOIL_load_image(this->filepath.c_str(), &width, &height, nullptr, SOIL_LOAD_RGBA);
  if (!image) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load texture: %s", this->filepath.c_str());
    return false;
  }

  glGenTextures(1, &this->id_);
  glBindTexture(GL_TEXTURE_2D, this->id_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  // TODO: Do we need?
  glGenerateMipmap(GL_TEXTURE_2D);

  SOIL_free_image_data(image);

  return true;
}

void Texture::bind() const {
  glBindTexture(GL_TEXTURE_2D, this->id_);
}
