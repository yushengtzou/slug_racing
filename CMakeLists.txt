cmake_minimum_required(VERSION 3.10)
project(slug_racing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(CMAKE_PREFIX_PATH "C:/msys64/mingw64")
    set(SDL2_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/SDL2")
    include_directories("${CMAKE_PREFIX_PATH}/include")
    include_directories("${CMAKE_PREFIX_PATH}/include/SDL2")
endif()

find_package(SDL2 REQUIRED)

add_executable(slug_racing
    src/main.cpp
    src/display.cpp
    src/game_flow.cpp
    src/game_state.h
    src/element.h
    src/slug.cpp
)

target_link_libraries(slug_racing 
    SDL2::SDL2main 
    SDL2::SDL2
) 