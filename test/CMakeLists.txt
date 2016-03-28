cmake_minimum_required(VERSION 2.5)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


# turn on testing
enable_testing()

# find the GTest package
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories("./mock")
include_directories("${CMAKE_SOURCE_DIR}/../")

file(GLOB sdiSrc "${CMAKE_SOURCE_DIR}/../*.cpp")
file(GLOB mockSrc "./mock/*.cpp")

file(GLOB testSources "tests/*.cpp")
message("Tests: ${testSources}")
foreach(testSource ${testSources})
  get_filename_component(program ${testSource} NAME_WE)

  message("Program Name: ${program}")

  add_executable(${program} ${sdiSrc} ${testSource} ${mockSrc})
  target_link_libraries(${program} ${GTEST_BOTH_LIBRARIES})
  add_test(TESTTESTTEST ${program})
endforeach()
