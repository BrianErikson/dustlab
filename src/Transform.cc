#include "Transform.h"

Transform::Transform(const glm::mat4 &rotation, const glm::mat4 &scale, const glm::mat4 &translation) :
    r{rotation}, s{scale}, t{translation}
{

}

void Transform::update() {
  this->combined = this->t * this->r * this->s;
}
