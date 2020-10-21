#include "common.h"

DustLabRegistry &DustLabRegistry::instance() {
  static DustLabRegistry instance;
  return instance;
}
