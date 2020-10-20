#pragma once
#include <string>

class Texture
{
public:
  Texture(const std::string &filepath);
  bool init();
  void bind() const;

  const std::string filepath;

private:
  GLuint id_;
};


