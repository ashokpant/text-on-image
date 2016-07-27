cmake_minimum_required(VERSION 2.8)
project(text_on_image)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)

set(SOURCE_FILES src/main.cpp)
add_executable(text_on_image ${SOURCE_FILES})
target_link_libraries(text_on_image ${OpenCV_LIBS})