#pragma once
#include <glm/glm.hpp>

class Transform
{
public:
  Transform() = default;

  void set(const glm::mat4 &mat);
  void set_translation(glm::vec3 pos);
  void set_rotation(float degrees);
  [[nodiscard]] float get_rotation() const;
  void set_scale(const glm::vec2 &scale);
  void update();
  const glm::mat4& get_matrix();

private:
  bool dirty_{true};
  glm::mat4 matrix{1.f};
  float rotation{0.f};
  glm::vec2 scale_{1.f, 1.f};
  glm::vec3 translation_{0.f};
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