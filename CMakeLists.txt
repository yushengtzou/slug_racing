cmake_minimum_required(VERSION 3.10)
project(slug_racing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(CMAKE_PREFIX_PATH "C:/msys64/mingw64")
    set(SDL2_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/SDL2")
    include_directories("${CMAKE_PREFIX_PATH}/include")
    include_directories("${CMAKE_PREFIX_PATH}/include/SDL2")
elseif(APPLE)
    # Add macOS-specific SDL2 include paths
    include_directories("/opt/homebrew/include")
    include_directories("/usr/local/include")
    # Add library search paths
    link_directories("/opt/homebrew/lib")
    link_directories("/usr/local/lib")
endif()

find_package(SDL2 REQUIRED)
find_package(RapidJSON REQUIRED)
include_directories(${RAPIDJSON_INCLUDE_DIRS})

find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(slug_racing
    src/main.cpp
    src/display.cpp
    src/game_flow.cpp
    src/game_state.h
    src/element.h
    src/slug.cpp
    src/end.h
    src/food.h
)

target_link_libraries(slug_racing 
    SDL2::SDL2main 
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
    SDL2_image::SDL2_image
) 