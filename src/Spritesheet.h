#pragma once
#include <string>
#include <glm/glm.hpp>
#include "Texture.h"

class Spritesheet : public Texture
{
public:
  explicit Spritesheet(const std::string &filepath, const Size<int> &sprite_size = {});
  ~Spritesheet() override;

  bool init(Size<int> sprite_size);
  bool init() override;
  void bind() const override;

  [[nodiscard]] int rows() const;
  [[nodiscard]] int cols() const;
  [[nodiscard]] glm::vec2 image_offset(int row, int col) const;
  [[nodiscard]] glm::vec3 model_offset(int row, int col) const;
  [[nodiscard]] Size<float> cell_size() const;

protected:
  Size<int> sprite_size_{0, 0};
  GLuint ss_ebo{0};
  GLuint ss_vao_{0};
  GLuint ss_vbo_{0};
  int subdiv_rows_{0};
  int subdiv_cols_{0};
  float stride_width_{0};
  float stride_height_{0};
};
