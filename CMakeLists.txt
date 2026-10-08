cmake_minimum_required(VERSION 3.15)


project(GB-Emulator VERSION 1.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(SDL2 REQUIRED)


add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/Cpu.cpp
    src/Memory.cpp
)


target_compile_options(${PROJECT_NAME} PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
    -Werror 
    -Wno-unused-parameter
)


target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
