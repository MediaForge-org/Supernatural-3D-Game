cmake_minimum_required(VERSION 3.25)

# Standalone generator of placeholder world source data (procedural textures, terrain) for the Unreal visual build.
# Separate from the SupernaturalCore project on purpose: a development tool, not game code.
#   cmake -S Tools/SpnWorldTool -B build/worldtool
#   cmake --build build/worldtool --config Release
project(SpnWorldTool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(spn_worldtool main.cpp textures.cpp terrain.cpp common.h)
if(MSVC)
    target_compile_options(spn_worldtool PRIVATE /W4 /permissive- /fp:fast)
else()
    target_compile_options(spn_worldtool PRIVATE -Wall -Wextra -O2)
    find_package(Threads REQUIRED)
    target_link_libraries(spn_worldtool PRIVATE Threads::Threads)
endif()
