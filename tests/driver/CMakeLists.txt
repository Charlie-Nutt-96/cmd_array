


add_executable(test_driver
    driver.cpp
)

target_include_directories(test_driver PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src
)