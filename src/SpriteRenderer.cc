#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "SpriteRenderer.h"
#include "ecs/core.h"
#include "Transform.h"

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

void SpriteRenderer::render(const Texture &tex, const glm::mat4 &model, const glm::vec3 &color) {
  const auto &p = this->registry_.projection().get_matrix();
  const auto &v = this->registry_.view().get_matrix();
  this->default_program_->use(p * v * model, color);
  this->quad_.bind();
  tex.bind();
  this->quad_.render();
}

void SpriteRenderer::render(const std::shared_ptr<Spritesheet> &ss, const glm::mat4 &model,
                            const glm::vec3 &color, int row, int col) {
  const auto &p = this->registry_.projection().get_matrix();
  const auto &v = this->registry_.view().get_matrix();
  //std::cout << "Model: " << glm::to_string(p * v * model) << std::endl;
  this->default_program_->use(p * v * model, color);
  ss->bind();
  ss->render(row, col);
}

void SpriteRenderer::render(const entt::entity &entity) {
  this->render(entity, glm::mat4{1.f});
}

void SpriteRenderer::render(const entt::entity &entity, const glm::mat4 &transform) {
  // TODO: get projection
  auto &local_t = this->registry_.ecs.get<ETransform>(entity);
  assert(local_t.type == TransformType::MODEL);
  std::cout << "Local: " << glm::to_string(local_t.t.get_matrix()) << std::endl;
  std::cout << "t: " << glm::to_string(transform) << std::endl;
  glm::mat4 combined_t = transform * local_t.t.get_matrix();
  std::cout << "combined: " << glm::to_string(combined_t) << std::endl;

  if (this->registry_.ecs.has<ESpritesheet>(entity)) {
    auto &ess = this->registry_.ecs.get<ESpritesheet>(entity);
    this->render(ess.value, combined_t, glm::vec3{1.f, 1.f, 1.f}, ess.row, ess.col);
  }

  // TODO if texture?

  if (this->registry_.ecs.has<EActor>(entity)) {
    for (const auto &sub_entity : this->registry_.ecs.get<EActor>(entity).children) {
      this->render(sub_entity, combined_t);
    }
  }

  //assert(false);
}
