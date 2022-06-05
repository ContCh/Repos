cmake_minimum_required(VERSION 3.15)
project(LOGGING)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O0 -Werror -Wall -fsanitize=address -fno-omit-frame-pointer")
set(SRC_FILES test.cpp log_base.cpp)
add_executable(LogTest ${SRC_FILES})