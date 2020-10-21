#pragma once
#include <glm/glm.hpp>

class Transform
{
public:
  Transform() = default;
  Transform(const glm::mat4 &rotation, const glm::mat4 &scale, const glm::mat4 &translation);

  void translate(glm::vec3 pos);
  void update();
  const glm::mat4& get_matrix();

private:
  bool dirty_{false};
  glm::mat4 matrix{1.f};
  glm::mat4 r{1.f};
  glm::mat4 s{1.f};
  glm::mat4 t{1.f};
};
