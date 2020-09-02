# Based on https://pytorch.org/tutorials/advanced/cpp_frontend.html#writing-a-basic-application
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(microbenchmark)

find_package(Torch REQUIRED)

set(CMAKE_VERBOSE_MAKEFILE ON)

# this doesn't seem to matter, but why not
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_executable(microbenchmark main.cpp)
target_link_libraries(microbenchmark "${TORCH_LIBRARIES}")
set_property(TARGET microbenchmark PROPERTY CXX_STANDARD 14)
