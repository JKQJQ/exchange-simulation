cmake_minimum_required(VERSION 2.8)

project(cuo)
add_compile_options(-std=c++17)
add_definitions(-O2)

include_directories(
    ${HDF5_INCLUDE_DIRS}
    ${zstd_INCLUDE_DIRS}
)

add_executable(merge_cuo merge_cuo.cpp)
