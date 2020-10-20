#pragma once
#include <memory>
#include "geometry/Quad.h"
#include "Texture.h"
#include "GLDefaultProgram.h"
#include "Spritesheet.h"

class SpriteRenderer
{
public:
  SpriteRenderer() = default;
  bool init();
  void render(const Texture &tex, const glm::mat4 &model, const glm::mat4 &projection, const glm::vec3 &color);
  void render(const Spritesheet &ss,
              const glm::mat4 &model,
              const glm::mat4 &projection,
              const glm::vec3 &color,
              int x,
              int y);

private:
  std::unique_ptr<GLDefaultProgram> default_program_{nullptr};
  Quad quad_{0};
};


