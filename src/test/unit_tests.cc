#include <gtest/gtest.h>
#include "DustLab.h"

TEST(dustlab, init) {
  DustLab dl;
  ASSERT_TRUE(dl.init());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}