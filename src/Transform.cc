#include <glm/gtx/transform.hpp>
#include "Transform.h"

Transform::Transform(const glm::mat4 &rotation, const glm::mat4 &scale, const glm::mat4 &translation) :
    r{rotation}, s{scale}, t{translation}
{

}

void Transform::translate(glm::vec3 pos) {
  this->t = glm::translate(this->t, pos);
  this->dirty_ = true;
}

void Transform::update() {
  this->matrix = this->t * this->r * this->s;
  this->dirty_ = false;
}

const glm::mat4& Transform::get_matrix() {
  if (this->dirty_) {
    this->update();
  }
  return this->matrix;
}
