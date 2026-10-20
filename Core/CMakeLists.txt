# Engine-independent core. Must never depend on Unreal Engine.
add_library(spn_core STATIC
    src/Appearance.cpp
    src/AppearanceEdit.cpp
    src/AppearanceGenerator.cpp
    src/AppearanceRecipe.cpp
    src/ContentId.cpp
    src/DetailTiers.cpp
    src/Interaction.cpp
    src/TestDoor.cpp
    src/WorldFacts.cpp)
add_library(spn::core ALIAS spn_core)

target_include_directories(spn_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_features(spn_core PUBLIC cxx_std_20)
target_link_libraries(spn_core PRIVATE spn_build_options)
