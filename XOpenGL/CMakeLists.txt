cmake_minimum_required(VERSION 3.8)

include_directories(../)
include_directories(../3rdpart/glad/include)

add_library(XGL Shader.h Shader.cpp stb_image.h stb_image.cpp)
