# Engine-independent core. Must never depend on Unreal Engine.
add_library(spn_core STATIC
    src/Appearance.cpp
    src/AppearanceEdit.cpp
    src/AppearanceGenerator.cpp
    src/AppearanceRecipe.cpp
    src/CameraArm.cpp
    src/ContentId.cpp
    src/DetailTiers.cpp
    src/Devices.cpp
    src/GameEvents.cpp
    src/GroundSteering.cpp
    src/Interaction.cpp
    src/Inventory.cpp
    src/InventoryPersistence.cpp
    src/InteractionSystem.cpp
    src/InteractionReach.cpp
    src/JumpModel.cpp
    src/Locomotion.cpp
    src/Minimap.cpp
    src/Movement.cpp
    src/Navigation.cpp
    src/Npc.cpp
    src/ObjectPersistence.cpp
    src/Pickup.cpp
    src/TestDoor.cpp
    src/Traversal.cpp
    src/SpatialGrid.cpp
    src/WorldFacts.cpp
    src/WorldMap.cpp)
add_library(spn::core ALIAS spn_core)

target_include_directories(spn_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_features(spn_core PUBLIC cxx_std_20)
target_link_libraries(spn_core PRIVATE spn_build_options)
