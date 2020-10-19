#pragma once
#include <string>

class GLShader
{
public:
  explicit GLShader(const std::string &filepath);
  ~GLShader();
  bool init();
  GLuint get_id();

private:
  const std::string filepath;
  GLenum shader_type{0};
  GLuint id_{0};
};


