cmake_minimum_required(VERSION 3.5)

project(Robot2024)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(External/eigen)

if(WIN32)
    set(WINSOCK ws2_32)
endif()

add_executable(Robot2024 Source/Main.cxx Source/Link.cxx)
target_include_directories(Robot2024 PRIVATE Include)
target_link_libraries(Robot2024 PRIVATE Eigen3::Eigen ${WINSOCK})
