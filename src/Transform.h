#pragma once
#include <glm/glm.hpp>

class Transform
{
public:
  Transform() = default;

  void set(const glm::mat4 &mat);
  void set_translation(glm::vec3 pos);
  void set_scale(float width, float height);
  void update();
  const glm::mat4& get_matrix();

private:
  bool dirty_{true};
  glm::mat4 matrix{1.f};
  glm::mat4 r{1.f};
  glm::mat4 s{1.f};
  glm::mat4 t{1.f};
};

enum class TransformType {
  MODEL,
  VIEW,
  PROJECTION
};

struct ETransform {
  Transform t{};
  TransformType type{TransformType::MODEL};
};