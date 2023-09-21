cmake_minimum_required(VERSION 3.5)

project(Robot2024)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

add_executable(Robot2024 Source/Main.cxx)
target_include_directories(Robot2024 PRIVATE Include)
