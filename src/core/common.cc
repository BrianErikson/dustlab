#include "common.h"
#include <Transform.h>
#include <glm/gtc/matrix_transform.hpp>

DustLabRegistry &DustLabRegistry::instance() {
  static DustLabRegistry instance;
  return instance;
}

DustLabRegistry::DustLabRegistry() {
  {
    auto &t = this->ecs.emplace<ETransform>(this->camera_);
    t.type = TransformType::PROJECTION;
    t.t.set(glm::ortho(-10.f, 10.f, -10.f, 10.f, 0.0f, 10.f));
  }

  {
    auto &t = this->ecs.emplace<ETransform>(this->world_);
    t.type = TransformType::VIEW;
    t.t.set(glm::mat4{1.f});
  }
}

entt::entity DustLabRegistry::camera() {
  return this->camera_;
}

entt::entity DustLabRegistry::world() {
  return this->world_;
}

Transform& DustLabRegistry::view() {
  return this->ecs.get<ETransform>(this->world_).t;
}

Transform& DustLabRegistry::projection() {
  return this->ecs.get<ETransform>(this->camera_).t;
}
