#pragma once
#include <entt/entt.hpp>
#include <Transform.h>

class DustLabRegistry {
public:
  static DustLabRegistry& instance();
  DustLabRegistry(DustLabRegistry const&) = delete;
  void operator=(DustLabRegistry const&) = delete;

  entt::entity camera();
  entt::entity world();
  Transform& view();
  Transform& projection();

  entt::registry ecs{};

private:
  DustLabRegistry();

  entt::entity camera_{this->ecs.create()};
  entt::entity world_{this->ecs.create()};
};
