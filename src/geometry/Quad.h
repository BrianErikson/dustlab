#pragma once
#include <GL/glew.h>
#include "../Renderable.h"

class Quad : public Renderable
{
public:
  Quad(int id);
  ~Quad() override;
  void render() override;
  bool init() override;
  void bind();

private:
  GLuint quad_vao{0};
  GLuint quad_vbo{0};
  GLuint quad_ibo{0};
};
