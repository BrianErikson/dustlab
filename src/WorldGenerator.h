#pragma once
#include <memory>

struct Image {
  std::shared_ptr<const unsigned char> data{nullptr};
  int width{0};
  int height{0};
};

class WorldGenerator
{
public:
  static Image get(int seed, double step, int x, int y, int z, int width, int height);
};


