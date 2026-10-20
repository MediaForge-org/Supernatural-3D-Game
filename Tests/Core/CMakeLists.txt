add_executable(spn_core_tests
    TestMain.cpp
    ResultTests.cpp
    StrongIdTests.cpp
    ContentIdTests.cpp
    AppearanceTests.cpp
    AppearanceEditTests.cpp
    AppearanceGeneratorTests.cpp
    AppearanceRecipeTests.cpp
    DetailTierTests.cpp
    DoorPersistenceTests.cpp
    InteractionTests.cpp
    WorldFactsTests.cpp
    WorldFactsCompatibilityTests.cpp)
# Frozen golden save files used by the compatibility tests.
target_compile_definitions(spn_core_tests PRIVATE
    SPN_TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Data")
# spn::doctest must come after spn_build_options: its -Wno-* flags have to
# follow -Wpedantic/-Wextra on the command line to take effect.
target_link_libraries(spn_core_tests PRIVATE spn::core spn_build_options spn::doctest)

doctest_discover_tests(spn_core_tests)
