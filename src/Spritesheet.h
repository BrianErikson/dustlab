#pragma once
#include <string>
#include "Texture.h"

class Spritesheet : public Texture
{
public:
  Spritesheet(const std::string &filepath, const Size &sprite_size);
  ~Spritesheet();

  bool init() override;
  void bind() const override;

  void render(int row, int col) const;
  const unsigned char* sprite(int row, int col);
  [[nodiscard]] int rows() const;
  [[nodiscard]] int cols() const;

  const Size sprite_size;

protected:
  GLuint ss_ebo{0};
  GLuint ss_vao_{0};
  GLuint ss_vbo_{0};
  int subdiv_rows_{0};
  int subdiv_cols_{0};
};


