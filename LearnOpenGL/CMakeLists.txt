cmake_minimum_required(VERSION 3.8)

include_directories(../3rdpart/glad/include)
include_directories(../3rdpart/glfw/include)

include_directories(../)

link_directories(../3rdpart/glfw/lib)
link_directories(../XOpenGL)

add_executable(LearnOpenGL 
    main.cpp
    ../3rdpart/glad/src/glad.c
    )
target_link_libraries(LearnOpenGL glfw3)
target_link_libraries(LearnOpenGL XGL)