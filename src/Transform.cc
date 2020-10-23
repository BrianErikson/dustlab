#include <glm/gtx/transform.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include "Transform.h"

void Transform::set(const glm::mat4 &mat) {
  this->matrix = mat;
  // TODO: derive components?
  this->translation_ = glm::vec3{0.f};
  this->rotation = 0.f;
  this->scale_ = glm::vec2{1.f, 1.f};
  this->dirty_ = false;
}

void Transform::set_translation(glm::vec3 pos) {
  this->translation_ = pos;
  this->dirty_ = true;
}

void Transform::set_rotation(float degrees) {
  this->rotation = glm::radians(degrees);
  this->dirty_ = true;
}

float Transform::get_rotation() const {
  return this->rotation;
}

void Transform::set_scale(const glm::vec2 &scale) {
  this->scale_ = scale;
  this->dirty_ = true;
}

void Transform::update() {
  glm::mat4 scale = glm::scale(glm::mat4{1.f}, glm::vec3{this->scale_.x, this->scale_.y, 1.f});
  glm::mat4 rotate = glm::rotate(this->rotation, glm::vec3{0.f, 0.f, 1.f});
  glm::mat4 translate = glm::translate(glm::mat4{1.f}, this->translation_);
  this->matrix = scale * rotate * translate;
  this->dirty_ = false;
}

const glm::mat4& Transform::get_matrix() {
  if (this->dirty_) {
    this->update();
  }
  return this->matrix;
}
