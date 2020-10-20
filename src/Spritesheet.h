#pragma once
#include <string>
#include "Texture.h"

class Spritesheet : public Texture
{
public:
  Spritesheet(const std::string &filepath, const Size &sprite_size);

  bool init() override;

  const unsigned char* sprite(int row, int col);
  [[nodiscard]] int rows() const;
  [[nodiscard]] int cols() const;

  const Size sprite_size;

protected:
  int subdiv_rows_{0};
  int subdiv_cols_{0};
};


