cmake_minimum_required(VERSION 3.13)
project(glad)
add_library(glad src/glad.c)
target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)
