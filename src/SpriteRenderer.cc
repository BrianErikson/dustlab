#include "SpriteRenderer.h"

bool SpriteRenderer::init() {
  this->default_program_ = std::make_unique<GLProgram>("./res/shaders/default.vert",
                                                       "./res/shaders/default.frag");
  if (!this->default_program_->init()) {
    return false;
  }

  if (!this->quad_.init()) {
    return false;
  }

  return true;
}
