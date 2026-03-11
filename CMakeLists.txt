cmake_minimum_required(VERSION 3.20)
project(LimitEngine LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/Limit)

add_executable(limit_engine main.cpp)
target_include_directories(limit_engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(limit_engine PRIVATE limit)

if(MSVC)
    target_compile_options(limit_engine PRIVATE /W4 /permissive- /utf-8)
endif()
