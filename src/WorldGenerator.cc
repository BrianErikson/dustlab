#include "WorldGenerator.h"
#include <noise/noise.h>
#include <algorithm>

using namespace noise;

Image WorldGenerator::get(int seed, double step, int x, int y, int z, int width, int height) {
  module::Perlin perlin{};
  perlin.SetNoiseQuality(NoiseQuality::QUALITY_BEST);
  perlin.SetSeed(seed);

  std::shared_ptr<unsigned char> image{static_cast<unsigned char *>(
      malloc(width * height * sizeof(unsigned char))), free};

  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      image.get()[width * row + col] = std::clamp(
          static_cast<unsigned int>(255u * ((perlin.GetValue(
              x + (step * col), y + (step * row), z) + 1.0) / 2.0)),
              0u, 255u);
    }
  }

  return {image, width, height};
}
