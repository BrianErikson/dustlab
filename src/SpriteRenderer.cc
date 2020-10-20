#include "SpriteRenderer.h"

bool SpriteRenderer::init() {
  this->default_program_ = std::make_unique<GLDefaultProgram>();
  if (!this->default_program_->init()) {
    return false;
  }

  if (!this->quad_.init()) {
    return false;
  }

  return true;
}

void SpriteRenderer::render(const Texture &tex, const glm::mat4 &model, const glm::mat4 &projection,
                            const glm::vec3 &color) {
  this->default_program_->use(model, projection, color);
  this->quad_.bind();
  tex.bind();
  this->quad_.render();
}

void SpriteRenderer::render(const Spritesheet &ss, const glm::mat4 &model, const glm::mat4 &projection,
                            const glm::vec3 &color, int x, int y) {
  this->default_program_->use(model, projection, color);
  this->quad_.bind();

  this->quad_.render();
}
