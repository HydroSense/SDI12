cmake_minimum_required(VERSION 3.4)

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
foreach(source ${testSources})
  get_filename_component(program ${source} NAME_WE)

  message("Program Name: ${program}")

  add_executable(${program} ${source} ${testSources} ${mockSrc})
  target_link_libraries(${program} ${GTEST_BOTH_LIBRARIES})
  add_test(TESTTESTTEST ${program})
endforeach()
