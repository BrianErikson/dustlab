#pragma once
#include <string>

class Texture
{
public:
  Texture(const std::string &filepath);
  bool init();

  const std::string filepath;
};


