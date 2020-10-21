#pragma once
#include <glm/glm.hpp>

struct Transform
{
  Transform() = default;
  Transform(const glm::mat4 &rotation, const glm::mat4 &scale, const glm::mat4 &translation);

  void update();

  glm::mat4 combined{1.f};
  glm::mat4 r{1.f};
  glm::mat4 s{1.f};
  glm::mat4 t{1.f};
};
