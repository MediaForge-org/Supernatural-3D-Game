cmake_minimum_required(VERSION 3.25)

project(SupernaturalCore
    VERSION 0.0.1
    DESCRIPTION "Engine-independent core of Supernatural 3D Game C++"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(SPN_BUILD_TESTS "Build the standalone Core tests" ON)
option(SPN_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(SPN_SANITIZE "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(SPN_ENABLE_CLANG_TIDY "Run clang-tidy during the build" OFF)

# Warning/sanitizer settings are carried by one interface target so that every
# SPN target opts in explicitly and third-party code is left untouched.
add_library(spn_build_options INTERFACE)
if(MSVC)
    target_compile_options(spn_build_options INTERFACE /W4 /permissive-)
    if(SPN_WARNINGS_AS_ERRORS)
        target_compile_options(spn_build_options INTERFACE /WX)
    endif()
else()
    target_compile_options(spn_build_options INTERFACE
        -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow)
    if(SPN_WARNINGS_AS_ERRORS)
        target_compile_options(spn_build_options INTERFACE -Werror)
    endif()
    if(SPN_SANITIZE)
        target_compile_options(spn_build_options INTERFACE
            -fsanitize=address,undefined -fno-sanitize-recover=undefined
            -fno-omit-frame-pointer)
        target_link_options(spn_build_options INTERFACE -fsanitize=address,undefined)
    endif()
endif()

if(SPN_ENABLE_CLANG_TIDY)
    find_program(SPN_CLANG_TIDY_EXE NAMES clang-tidy REQUIRED)
    set(CMAKE_CXX_CLANG_TIDY ${SPN_CLANG_TIDY_EXE})
endif()

add_subdirectory(Core)

if(SPN_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()
