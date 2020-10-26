#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "SpriteRenderer.h"
#include "ecs/core.h"
#include "Transform.h"
#include <core/common.h>

SpriteRenderer::SpriteRenderer() : registry_{DustLabRegistry::instance()} {

}

bool SpriteRenderer::init() {
  if (this->default_program_) {
    return true;
  }

  this->default_program_ = std::make_unique<GLDefaultProgram>();
  if (!this->default_program_->init()) {
    return false;
  }

  return true;
}

void SpriteRenderer::render(const Scene &scene, const glm::mat4 &model, const glm::vec3 &color) {
  static const int stride{4};
  const auto &p = this->registry_.projection().get_matrix();
  const auto &v = this->registry_.view().get_matrix();

  this->default_program_->use(p * v * model, color);
  scene.tilesheet->bind();

  const auto &cell_size = scene.tilesheet->cell_size();
  Transform cur_t;
  for (int row = 0; row < scene.map.rows; row++) {
    for (int col = 0; col < scene.map.cols; col++) {
      const auto &value = scene.map.at<int32_t>(row, col);
      if (value & TileFlags::MEDIUM_VOID) {
        continue;
      }
      cur_t.set_translation({col * cell_size.width, row * cell_size.height, 0.f});
      auto &tile = scene.tilesheet->tile(0, value);
      this->default_program_->set_mvp(p * v * cur_t.get_matrix() * tile.transform.get_matrix());
      const unsigned long offset{(scene.tilesheet->cols() * tile.row + tile.col) * stride * sizeof(GLubyte)};
      glDrawElements(GL_TRIANGLE_STRIP, stride, GL_UNSIGNED_BYTE, reinterpret_cast<const void *>(offset));
    }
  }
}

void SpriteRenderer::render(const Spritesheet *ss, const glm::mat4 &model, const glm::vec3 &color,
                            int row, int col) {
  static const int stride{4};
  const auto &p = this->registry_.projection().get_matrix();
  const auto &v = this->registry_.view().get_matrix();
  const unsigned long offset{(ss->cols() * row + col) * stride * sizeof(GLubyte)};

  this->default_program_->use(p * v * model, color);
  ss->bind();
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDrawElements(GL_TRIANGLE_STRIP, stride, GL_UNSIGNED_BYTE, reinterpret_cast<const void *>(offset));
}

void SpriteRenderer::render(const entt::entity &entity) {
  this->render(entity, glm::mat4{1.f});
}

void SpriteRenderer::render(const entt::entity &entity, const glm::mat4 &transform) {
  // TODO: get projection
  auto &local_t = this->registry_.ecs.get<ETransform>(entity);
  assert(local_t.type == TransformType::MODEL);
  glm::mat4 combined_t = transform * local_t.t.get_matrix();

  if (this->registry_.ecs.has<ESpriteModel>(entity)) {
    auto &ess = this->registry_.ecs.get<ESpriteModel>(entity);
    this->render(ess.value.get(), ess.transform.get_matrix() * combined_t, glm::vec3{1.f, 1.f, 1.f}, ess.row, ess.col);
  }

  // TODO if texture?

  if (this->registry_.ecs.has<EActor>(entity)) {
    for (const auto &sub_entity : this->registry_.ecs.get<EActor>(entity).children) {
      this->render(sub_entity, combined_t);
    }
  }

  //assert(false);
}
