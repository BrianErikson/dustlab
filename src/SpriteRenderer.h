#pragma once
#include <memory>
#include "GLProgram.h"
#include "geometry/Quad.h"

class SpriteRenderer
{
public:
  SpriteRenderer() = default;
  bool init();
private:
  std::unique_ptr<GLProgram> default_program_{nullptr};
  Quad quad_{0};

};


