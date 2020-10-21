#include <glm/gtx/transform.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include "Transform.h"

void Transform::set(const glm::mat4 &mat) {
  this->matrix = mat;
  // TODO: derive components?
  this->t = glm::mat4{1.f};
  this->r = glm::mat4{1.f};
  this->s = glm::mat4{1.f};
  this->dirty_ = false;
}

void Transform::set_translation(glm::vec3 pos) {
  this->t = glm::translate(glm::mat4{1.f}, pos);
  this->dirty_ = true;
}

void Transform::set_scale(float width, float height) {
  this->s = glm::scale(glm::mat4{1.f}, {width, height, 1.f});
  this->dirty_ = true;
}

void Transform::update() {
  this->matrix = this->s * this->r * this->t;
  this->dirty_ = false;
}

const glm::mat4& Transform::get_matrix() {
  if (this->dirty_) {
    this->update();
  }
  return this->matrix;
}
