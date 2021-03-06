cmake_minimum_required(VERSION 3.13)
project(dustlab)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})
add_compile_options(-Wall -pedantic -fconcepts)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
find_package(SOIL REQUIRED)
find_package(EnTT REQUIRED)
find_package(LibNoise REQUIRED)
find_package(cmcstl2 CONFIG REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(glew REQUIRED IMPORTED_TARGET glew)

set(SRCS
  src/DustLab.cc
  src/Renderable.cc
  src/geometry/Quad.cc
  src/Renderable.cc
  src/GLShader.cc
  src/GLProgram.cc
  src/SpriteRenderer.cc
  src/Texture.cc
  src/GLDefaultProgram.cc
  src/Spritesheet.cc
  src/Transform.cc
  src/ecs/core.cc
  src/core/DustLabRegistry.cc
  src/ecs/EntityGenerator.cc
  src/Tilesheet.cc
  src/WorldGenerator.cc
)

add_library(dustlabObj OBJECT
  ${SRCS}
)

target_include_directories(dustlabObj PUBLIC
  ${SOIL_INCLUDE_DIRS}
  ${glew_INCLUDEDIR}
  ${LIBNOISE_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/extern
)

target_link_libraries(dustlabObj
  SDL2
  stl2
  ${LIBNOISE_LIBRARIES}
  ${EnTT_LIBRARIES}
  ${glm_LIBRARIES}
  ${SOIL_LIBRARIES}
  ${OpenCV_LIBS}
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
