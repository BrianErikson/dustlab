#pragma once

template<class T>
struct Size {
  T width{0};
  T height{0};
  Size() = default;
  Size(T width, T height) : width(width), height(height) {}
  Size(const Size &other) = default;
};