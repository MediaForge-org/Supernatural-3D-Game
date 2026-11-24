add_executable(spn_core_tests
    TestMain.cpp
    ResultTests.cpp
    StrongIdTests.cpp
    CameraArmTests.cpp
    ContentIdTests.cpp
    SaveSlotTests.cpp
    GameSaveTests.cpp
    CoordinateTests.cpp
    AppearanceTests.cpp
    AppearanceEditTests.cpp
    AppearanceGeneratorTests.cpp
    AppearanceRecipeTests.cpp
    DetailTierTests.cpp
    DevicesTests.cpp
    GameEventsTests.cpp
    DoorPersistenceTests.cpp
    DoorStatesTests.cpp
    EnvScenarioTests.cpp
    InteractionTests.cpp
    InventoryTests.cpp
    InventoryTxTests.cpp
    CatalogFingerprintTests.cpp
    InteractionReachTests.cpp
    InteractionSystemTests.cpp
    JumpModelTests.cpp
    LedgeAssistTests.cpp
    BodyCoverageTests.cpp
    GroundSteeringTests.cpp
    LocomotionTests.cpp
    MapGeometryTests.cpp
    MapRenderTests.cpp
    NavigationSessionTests.cpp
    RoutingTests.cpp
    MapTests.cpp
    MinimapTests.cpp
    MovementTests.cpp
    NavigationTests.cpp
    NpcMemoryTests.cpp
    NpcReactionTests.cpp
    NpcSaveTests.cpp
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

add_executable(spn_core_bench_navigation BenchNavigation.cpp)
target_link_libraries(spn_core_bench_navigation PRIVATE spn::core spn_build_options)

add_executable(spn_core_bench_m2b1 BenchM2B1.cpp)
target_link_libraries(spn_core_bench_m2b1 PRIVATE spn::core spn_build_options)

# Guard against vacuous CTest entries (see CheckTestNames.cmake).
add_test(NAME spn_core_test_names
    COMMAND ${CMAKE_COMMAND} -DTESTS=$<TARGET_FILE:spn_core_tests> -P ${CMAKE_CURRENT_SOURCE_DIR}/CheckTestNames.cmake)

# Hand-run: spn_core_save_probe <dir> reports whether the save protocol's filesystem primitives work on that volume.
add_executable(spn_core_save_probe SaveProbe.cpp)
target_link_libraries(spn_core_save_probe PRIVATE spn::core spn_build_options)

# Hand-run: inventory transaction latency / allocations before vs after an implementation change (Release build).
add_executable(spn_core_bench_inventory_tx BenchInventoryTx.cpp)
target_link_libraries(spn_core_bench_inventory_tx PRIVATE spn::core spn_build_options)

# Hand-run (Windows only): spn_core_save_kill_test run <dir> [iterations] [seed] [maxDelayMs] terminates a saving child process at random
# moments and checks what can be loaded afterwards. Not registered with CTest (it is slow and uses real files).
if(WIN32)
    add_executable(spn_core_save_kill_test SaveKillTest.cpp)
    target_link_libraries(spn_core_save_kill_test PRIVATE spn::core spn_build_options)
endif()
