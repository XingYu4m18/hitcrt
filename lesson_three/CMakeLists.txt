# CMake 构建入口。命令行执行 cmake -S . -B build 即可生成构建文件。
cmake_minimum_required(VERSION 3.16)

project(rm_target_demo LANGUAGES C)

# 全工程统一使用 C11，并关闭编译器对 GNU 扩展的默认支持，保证代码更标准。
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# 生成 compile_commands.json，供编辑器或 clangd 等工具理解每个源文件的编译参数。
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# main.c 负责菜单交互，target.c 负责目标数据的过滤、排序和显示。
add_executable(rm_target_demo_1
    src/main.c
    src/target.c
)

# 把 include 目录加入头文件搜索路径，这样源码中可以直接写 #include "target.h"。
target_include_directories(
    rm_target_demo_1
    PRIVATE
    include
)

