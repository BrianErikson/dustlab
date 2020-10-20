#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <SOIL.h>
#include <GL/glew.h>
#include "geometry/common.h"

class Texture
{
public:
  Texture(const std::string &filepath);
  virtual ~Texture() = default;
  virtual bool init();
  virtual void bind() const;
  std::shared_ptr<const unsigned char> data();
  Size& size();

  const std::string filepath;

protected:
  Size size_;
  std::shared_ptr<const unsigned char> image_{nullptr, SOIL_free_image_data};
  GLuint id_{0};
};


