#pragma once
#include <memory>
#include <string>
#include <GL/glew.h>
#include "GLShader.h"

class GLProgram
{
public:
  GLProgram(const std::string &vert_path, const std::string &frag_path);
  virtual ~GLProgram();
  virtual bool init();
  virtual void use() const;

  const std::string vert_path;
  const std::string frag_path;

protected:
  GLuint id_{0};
  std::unique_ptr<GLShader> vertex_shader_{nullptr};
  std::unique_ptr<GLShader> fragment_shader_{nullptr};
};


