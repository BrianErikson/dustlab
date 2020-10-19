include(FetchContent)
FetchContent_Declare(googletest
GIT_REPOSITORY https://github.com/google/googletest.git
GIT_TAG release-1.10.0)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

add_executable(unit_tests
  unit_tests.cc
)
add_dependencies(unit_tests dustlabObj gtest gtest_main)

target_include_directories(unit_tests PUBLIC
  ${CMAKE_SOURCE_DIR}/src
  $<TARGET_PROPERTY:gtest,INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>
  $<TARGET_PROPERTY:gtest_main,INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>
)

target_link_libraries(unit_tests
  dustlabObj
  gtest
  gtest_main
)