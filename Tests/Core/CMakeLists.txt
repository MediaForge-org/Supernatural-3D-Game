add_executable(spn_core_tests
    TestMain.cpp
    ResultTests.cpp
    StrongIdTests.cpp
    WorldFactsTests.cpp)
# spn::doctest must come after spn_build_options: its -Wno-* flags have to
# follow -Wpedantic/-Wextra on the command line to take effect.
target_link_libraries(spn_core_tests PRIVATE spn::core spn_build_options spn::doctest)

doctest_discover_tests(spn_core_tests)
