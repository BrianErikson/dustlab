#pragma once

struct Size {
  int width{0};
  int height{0};
  Size() = default;
  Size(int width, int height) : width(width), height(height) {}
  Size(const Size &other) = default;
};