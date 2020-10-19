#include "DustLab.h"

int main() {
  DustLab dustlab;
  if (!dustlab.init()) {
    return 1;
  }
  return dustlab.run();
}
