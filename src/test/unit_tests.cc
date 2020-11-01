#include <gtest/gtest.h>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/string_cast.hpp>
#include "DustLab.h"

using namespace glm;

TEST(dustlab, init) {
  DustLab dl;
  ASSERT_TRUE(dl.init());
}

TEST(dustlab, transform) {
  {
    Transform t;
    ASSERT_TRUE(t.get_matrix() == mat4{1.f});
  }
  {
    Transform t;
    t.set_translation({1, 0});
    ASSERT_TRUE(t.get_matrix() == translate(mat4{1.f}, {1.f, 0.f}));
  }
  {
    Transform t;
    t.set_rotation(90);
    auto rhs = rotate(mat4{1.f}, radians(90.f));

    ASSERT_TRUE(t.get_matrix() == rhs) << "lhs:" << to_string(t.get_matrix())
      << "\nrhs:" << to_string(rhs);
  }
  {
    Transform t;
    t.set_scale(vec2{1.f});
    auto rhs = mat4{1.f};
    ASSERT_TRUE(t.get_matrix() == scale(rhs, {1.f, 1.f})) << "lhs:" << to_string(t.get_matrix())
                                                          << "\nrhs:" << to_string(scale(rhs, vec2{1.f, 1.f}));
  }
  {
    const vec2 scale{2.f, 2.f};
    const float rot{90.f};
    const vec2 pos{1.f, 0.f};

    Transform t;
    t.set_scale(scale);
    t.set_rotation(rot);
    t.set_translation(pos);

    mat4 rhs{1.f};
    rhs = glm::scale(rhs, scale);
    rhs = glm::rotate(rhs, glm::radians(rot));
    rhs = glm::translate(rhs, pos);

    ASSERT_EQ(t.get_matrix(), rhs) << "lhs:" << to_string(t.get_matrix())
                                   << "\nrhs:" << to_string(rhs);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}