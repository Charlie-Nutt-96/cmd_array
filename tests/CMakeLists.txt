cmake_minimum_required(VERSION 3.16)
project(ArrayTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

option(DRIVER OFF)
option(TEST ON)

if(TEST)
    add_subdirectory(unit_tests)
endif(TEST)

if(DRIVER)
    add_subdirectory(driver)
endif(DRIVER)