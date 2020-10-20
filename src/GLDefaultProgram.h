#pragma once
#include <glm/glm.hpp>
#include "GLProgram.h"

class GLDefaultProgram : public GLProgram
{
public:
  GLDefaultProgram();
  bool init() override;
  void use(const glm::mat4 &model, const glm::mat4 &projection, const glm::vec3 &color);

private:
  int uloc_model_{0}; // mat4
  int uloc_projection_{0}; // mat4
  int uloc_sprite_color_{0}; // vec3
};


