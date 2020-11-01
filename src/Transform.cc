#include <glm/gtx/string_cast.hpp>
#include "Transform.h"

using namespace glm;

void Transform::set(const mat4 &mat) {
  this->matrix_ = mat;
  // TODO: derive components?
  this->translation_ = vec3{0.f};
  this->rotation_ = 0.f;
  this->scale_ = vec3{1.f, 1.f, 1.f};
  this->dirty_ = false;
}

void Transform::set_translation(glm::vec2 pos) {
  this->translation_ = vec3{pos, 0.f};
  this->dirty_ = true;
}

void Transform::set_translation(vec3 pos) {
  this->translation_ = pos;
  this->dirty_ = true;
}

void Transform::set_rotation(float degrees) {
  this->rot_axis_ = glm::vec3{0.f, 0.f, 1.f};
  this->rotation_ = radians(degrees);
  this->dirty_ = true;
}

void Transform::set_rotation(float degrees, glm::vec3 axis) {
  this->rot_axis_ = axis;
  this->rotation_ = radians(degrees);
  this->dirty_ = true;
}

float Transform::get_rotation() const {
  return degrees(this->rotation_);
}

void Transform::set_scale(const vec2 &scale) {
  this->scale_ = vec3(scale, 1.f);
  this->dirty_ = true;
}

void Transform::set_scale(const vec3 &scale) {
  this->scale_ = scale;
  this->dirty_ = true;
}

void Transform::update() {
  this->matrix_ = mat4{1.f};
  this->matrix_ = scale(this->matrix_, this->scale_);
  this->matrix_ = rotate(this->matrix_, this->rotation_, this->rot_axis_);
  this->matrix_ = translate(this->matrix_, this->translation_);
  this->dirty_ = false;
}

const mat4& Transform::get_matrix() {
  if (this->dirty_) {
    this->update();
  }
  return this->matrix_;
}
