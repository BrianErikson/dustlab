#pragma once
#include <memory>
#include <entt/entt.hpp>
#include "geometry/Quad.h"
#include "Texture.h"
#include "GLDefaultProgram.h"
#include "Spritesheet.h"
#include "core/common.h"
#include "Transform.h"

class SpriteRenderer
{
public:
  SpriteRenderer() = default;
  bool init();
  void render(const Texture &tex, const glm::mat4 &model, const glm::mat4 &projection, const glm::vec3 &color);
  void render(const std::shared_ptr<Spritesheet> &ss, const glm::mat4 &model, const glm::mat4 &projection,
              const glm::vec3 &color, int row, int col);
  void render(const entt::entity &entity);
  void render(const entt::entity &entity, const glm::mat4 &transform);

private:
  DustLabRegistry &registry_{DustLabRegistry::instance()};
  std::unique_ptr<GLDefaultProgram> default_program_{nullptr};
  Quad quad_{0};
};
