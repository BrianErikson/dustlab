#pragma once
#include <memory>
#include <string>
#include "GLShader.h"

class GLProgram
{
public:
  GLProgram(const std::string &vert_path, const std::string &frag_path);
  ~GLProgram();
  bool init();
  void use() const;

  const std::string vert_path;
  const std::string frag_path;

private:
  GLuint id_{0};
  std::unique_ptr<GLShader> vertex_shader_{nullptr};
  std::unique_ptr<GLShader> fragment_shader_{nullptr};
};


