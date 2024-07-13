# GTest include code would
# go somewhere in here
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)


add_executable(cmda_tests cmda_tests.cpp)

target_link_libraries(cmda_tests
    PRIVATE
    GTest::GTest
)

target_include_directories(cmda_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src
)

add_test(cmda_tests cmda_tests)