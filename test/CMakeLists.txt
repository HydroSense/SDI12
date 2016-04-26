cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -std=c++11 -pthread")

# set up dependencies
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories("./arduino-mock")
file(GLOB arduinoMockSrc "./arduino-mock/src/*.cc")

# include the units under test
include_directories("../")
file(GLOB sdiSrc "${CMAKE_SOURCE_DIR}/../*.cpp")

# build the tests
file(GLOB testSources "*.cpp")
message("Tests: ${testSources}")
foreach(testSource ${testSources})
  get_filename_component(program ${testSource} NAME_WE)

  message("Program Name: ${program}")

  add_executable(${program} ${arduinoMockSrc} ${sdiSrc} ${testSource})
  target_link_libraries(${program} ${GTEST_BOTH_LIBRARIES} gmock gmock_main)
endforeach()
