#pragma once
#include <entt/entt.hpp>
#include <Spritesheet.h>
#include <Transform.h>

struct ESpriteModel {
  std::shared_ptr<Spritesheet> value{nullptr};
  Transform transform;
  int row{0};
  int col{0};
};

struct EActor {
  std::vector<entt::entity> children;
};

struct EName {
  std::string value;
};
