#pragma once
#include <optional>
#include <entt/entt.hpp>
#include <core/DustLabRegistry.h>
#include <Spritesheet.h>

class EntityGenerator
{
public:
  std::optional<entt::entity> BasicBot();

private:
  std::shared_ptr<Spritesheet> witchcraft_spritesheet_{
    new Spritesheet{"./res/textures/witchcraft_spritesheet.png", {24, 24}}};
  DustLabRegistry &registry_{DustLabRegistry::instance()};
};
