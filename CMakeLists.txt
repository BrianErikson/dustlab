cmake_minimum_required(VERSION 3.13)
project(dustlab)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})
add_compile_options(-Wall -pedantic)

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
find_package(SOIL REQUIRED)
find_package(EnTT REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(glew REQUIRED IMPORTED_TARGET glew)

set(SRCS
  src/DustLab.cc
  src/Renderable.cc src/Renderable.h src/geometry/Quad.cc src/geometry/Quad.h src/Renderable.cc src/GLShader.cc src/GLShader.h src/GLProgram.cc src/GLProgram.h src/SpriteRenderer.cc src/SpriteRenderer.h src/Texture.cc src/Texture.h src/GLDefaultProgram.cc src/GLDefaultProgram.h src/Spritesheet.cc src/Spritesheet.h src/geometry/common.h src/Transform.cc src/Transform.h src/ecs/core.h src/ecs/core.cc src/core/common.cc src/core/common.h extern/timer-wheel.h)

add_library(dustlabObj OBJECT
  ${SRCS}
)

target_include_directories(dustlabObj PUBLIC
  ${SOIL_INCLUDE_DIRS}
  ${glew_INCLUDEDIR}
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/extern
)

target_link_libraries(dustlabObj
  SDL2
  ${EnTT_LIBRARIES}
  ${glm_LIBRARIES}
  ${SOIL_LIBRARIES}
  PkgConfig::glew
)

add_executable(dustlab
  src/main.cpp
)

target_link_libraries(dustlab
  dustlabObj
)

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/dustlab-data/res
  ${CMAKE_BINARY_DIR}/res)

add_subdirectory(src/test)
