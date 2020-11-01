#pragma once
#include <glm/glm.hpp>
#include "GLProgram.h"

class GLDefaultProgram : public GLProgram
{
public:
  GLDefaultProgram();
  bool init() override;
  void use(const glm::mat4 &mvp, const glm::vec3 &color);
  void set_mvp(const glm::mat4 &mvp);

private:
  glm::vec3 color_{1.f};
  glm::mat4 mvp_{1.f};
  int uloc_mvp_{0}; // mat4
  int uloc_sprite_color_{0}; // vec3
};


