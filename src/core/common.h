#pragma once
#include <entt/entt.hpp>

class DustLabRegistry {
public:
  static DustLabRegistry& instance();
  DustLabRegistry(DustLabRegistry const&) = delete;
  void operator=(DustLabRegistry const&) = delete;

  entt::registry ecs{};

private:
  DustLabRegistry() = default;
};
