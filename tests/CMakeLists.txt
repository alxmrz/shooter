find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

add_executable(tests
    GameState_test.cpp
    GameObjects_test.cpp
    Fabric_test.cpp
    EventHandler_test.cpp
    AI_test.cpp
    Ground_test.cpp
    GroundBlock_test.cpp
    GroundBlockCollision_test.cpp
    EnemyCollision_test.cpp
    SpacingGrid_test.cpp
    Crystal_test.cpp
    Heart_test.cpp
    Ammunition_test.cpp
    Platform_test.cpp
    Movable_test.cpp
    Unit_test.cpp
    Shooter_test.cpp
    ShooterSprite_test.cpp
    Bullet_test.cpp
    BulletAim_test.cpp
    BulletExplosion_test.cpp
    Aim_test.cpp
    PlayerFiring_test.cpp
    Dialogue_test.cpp
    DialoguePause_test.cpp
    LevelGenerator_test.cpp
    PlatformCollision_test.cpp
    ShooterPlatformCollision_test.cpp
    Tree_test.cpp
)

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tests shooter_lib GTest::gtest_main sfml-graphics sfml-window sfml-system sfml-audio)

include(GoogleTest)
gtest_discover_tests(tests)
