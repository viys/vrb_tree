cmake_minimum_required(VERSION 3.15)

#c99
set(CMAKE_C_FLAGS "-std=c99")

# set the project name
project(vrb_tree)

# 将可执行文件输出到指定路径
# set(HOME ..)
# set(EXECUTABLE_OUTPUT_PATH ${HOME}/bin)

# add the executable
file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/app/*.c)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/app/bsp)

add_executable(vrb_tree ${SRC})
