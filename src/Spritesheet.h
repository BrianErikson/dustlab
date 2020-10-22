#pragma once
#include <string>
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include <core/common.h>
#include "Texture.h"

class Spritesheet : public Texture
{
public:
  Spritesheet(const std::string &filepath, const Size<int> &sprite_size);
  ~Spritesheet() override;

  bool init() override;
  void bind() const override;

  void render(int row, int col) const;
  [[nodiscard]] int rows() const;
  [[nodiscard]] int cols() const;
  [[nodiscard]] glm::vec2 image_offset(int row, int col) const;
  [[nodiscard]] glm::vec3 model_offset(int row, int col) const;
  [[nodiscard]] Size<float> cell_size() const;

protected:
  DustLabRegistry &registry_{DustLabRegistry::instance()};
  Size<int> sprite_size_{0, 0};
  GLuint ss_ebo{0};
  GLuint ss_vao_{0};
  GLuint ss_vbo_{0};
  int subdiv_rows_{0};
  int subdiv_cols_{0};
  float stride_width_{0};
  float stride_height_{0};
};
