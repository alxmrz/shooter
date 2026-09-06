find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

add_executable(tests
    GameState_test.cpp
    GameObjects_test.cpp
    Fabric_test.cpp
    EventHandler_test.cpp
)

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tests shooter_lib GTest::gtest_main sfml-graphics sfml-window sfml-system sfml-audio)

include(GoogleTest)
gtest_discover_tests(tests)
