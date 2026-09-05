cmake_minimum_required(VERSION 3.16)

project(
    AdaptiveTrafficIntelligence
    VERSION 1.0
    DESCRIPTION "Computer vision based adaptive traffic analysis prototype"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED)

add_executable(
    adaptive_traffic
    src/main.cpp
    src/traffic_analyzer.cpp
)

target_include_directories(
    adaptive_traffic
    PRIVATE
    include
)

target_link_libraries(
    adaptive_traffic
    PRIVATE
    ${OpenCV_LIBS}
)

if(MSVC)
    target_compile_options(
        adaptive_traffic
        PRIVATE
        /W4
    )
else()
    target_compile_options(
        adaptive_traffic
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()
