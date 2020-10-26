#pragma once
#include <memory>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <core/common.h>
#include "geometry/Quad.h"
#include "Texture.h"
#include "GLDefaultProgram.h"
#include "Spritesheet.h"
#include "Transform.h"
#include "WorldGenerator.h"
#include "Tilesheet.h"

struct Scene {
  Map map;
  std::shared_ptr<Tilesheet> tilesheet;
};

class SpriteRenderer
{
public:
  SpriteRenderer();
  bool init();
  void render(const Scene &scene, const glm::mat4 &model, const glm::vec3 &color);
  void render(const Spritesheet *ss, const glm::mat4 &model, const glm::vec3 &color,
              int row, int col);
  void render(const entt::entity &entity);
  void render(const entt::entity &entity, const glm::mat4 &transform);

private:
  std::unique_ptr<GLDefaultProgram> default_program_{nullptr};
  DustLabRegistry &registry_;
};
