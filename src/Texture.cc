#include <SOIL.h>
#include <GL/glew.h>
#include "Texture.h"

Texture::Texture(const std::string &filepath) : filepath(filepath) {

}

bool Texture::init() {
  int width, height;
  unsigned char *image = SOIL_load_image(this->filepath.c_str(), &width, &height, nullptr, SOIL_LOAD_RGBA);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  
}
