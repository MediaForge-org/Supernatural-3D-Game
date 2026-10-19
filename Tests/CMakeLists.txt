include(FetchContent)

# doctest, pinned to the commit of release v2.4.12 (MIT license).
# Verify: git ls-remote --tags https://github.com/doctest/doctest.git v2.4.12
# SOURCE_SUBDIR points to a nonexistent directory on purpose: doctest's own
# CMakeLists is not used (its minimum-version policy is outdated for CMake 4);
# only the header and the CTest discovery script are consumed below.
FetchContent_Declare(doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG        1da23a3e8119ec5cce4f9388e91b065e20bf06f5 # v2.4.12
    SOURCE_SUBDIR  spn-do-not-add-subdirectory)
FetchContent_MakeAvailable(doctest)

add_library(spn_doctest INTERFACE)
add_library(spn::doctest ALIAS spn_doctest)
target_include_directories(spn_doctest SYSTEM INTERFACE ${doctest_SOURCE_DIR}/doctest)

# By default doctest forward-declares std::basic_ostream instead of including
# <ostream>. Stringifying std::string_view then instantiates MSVC's
# operator<< on an incomplete type (C2027). Make doctest include the real
# standard headers on every platform (libc++ already forces this).
target_compile_definitions(spn_doctest INTERFACE DOCTEST_CONFIG_USE_STD_HEADERS)

# doctest v2.4.12 expands __COUNTER__ and includes <ciso646>, which newer
# compilers flag. Silence exactly these third-party diagnostics for consumers
# of this target (the test executables) only; Core keeps the full warning set.
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(spn_doctest INTERFACE "-Wno-c2y-extensions" "-Wno-#warnings")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(spn_doctest INTERFACE "-Wno-cpp")
endif()

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

add_subdirectory(Core)
