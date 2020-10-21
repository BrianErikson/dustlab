#pragma once
#include <entt/entt.hpp>

struct EActor {
  std::vector<entt::entity> children;
};

struct EName {
  std::string value;
};
