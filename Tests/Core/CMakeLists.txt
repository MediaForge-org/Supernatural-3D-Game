add_executable(spn_core_tests
    TestMain.cpp
    ResultTests.cpp
    StrongIdTests.cpp
    CameraArmTests.cpp
    ContentIdTests.cpp
    AppearanceTests.cpp
    AppearanceEditTests.cpp
    AppearanceGeneratorTests.cpp
    AppearanceRecipeTests.cpp
    DetailTierTests.cpp
    DevicesTests.cpp
    GameEventsTests.cpp
    DoorPersistenceTests.cpp
    EnvScenarioTests.cpp
    InteractionTests.cpp
    InventoryTests.cpp
    InteractionReachTests.cpp
    InteractionSystemTests.cpp
    JumpModelTests.cpp
    GroundSteeringTests.cpp
    LocomotionTests.cpp
    MapTests.cpp
    MinimapTests.cpp
    MovementTests.cpp
    NavigationTests.cpp
    NpcTests.cpp
    ObjectPersistenceTests.cpp
    PickupTests.cpp
    PropertyTests.cpp
    TraversalTests.cpp
    WorldFactsTests.cpp
    WorldFactsCompatibilityTests.cpp)
# Frozen golden save files used by the compatibility tests.
target_compile_definitions(spn_core_tests PRIVATE
    SPN_TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Data")
# spn::doctest must come after spn_build_options: its -Wno-* flags have to
# follow -Wpedantic/-Wextra on the command line to take effect.
target_link_libraries(spn_core_tests PRIVATE spn::core spn_build_options spn::doctest)

doctest_discover_tests(spn_core_tests)

# Hand-run micro-benchmarks (not registered with CTest).
add_executable(spn_core_bench Bench.cpp)
target_link_libraries(spn_core_bench PRIVATE spn::core spn_build_options)

add_executable(spn_core_bench_m2b1 BenchM2B1.cpp)
target_link_libraries(spn_core_bench_m2b1 PRIVATE spn::core spn_build_options)
