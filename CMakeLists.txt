cmake_minimum_required(VERSION 4.2)
project(video2text)
set(CMAKE_CXX_STANDARD 20)
find_package(OpenCV REQUIRED)
add_executable(video2text main.cpp)
target_link_libraries(video2text ${OpenCV_LIBS})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

