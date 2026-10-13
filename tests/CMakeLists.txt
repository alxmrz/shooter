find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

add_executable(tests
    AI_test.cpp
    DeathDialog_test.cpp
    DialoguePause_test.cpp
    EventHandler_test.cpp
    Fabric_test.cpp
    Flag_test.cpp
    FlagDialogue_test.cpp
    GameObjects_test.cpp
    GameState_test.cpp
    LevelGenerator_test.cpp
    SpacingGrid_test.cpp
    StageProgress_test.cpp

    objects/backgrounds/Ground_test.cpp
    objects/backgrounds/GroundBlock_test.cpp
    objects/backgrounds/GroundBlockCollision_test.cpp

    objects/interactive/Aim_test.cpp
    objects/interactive/Ammunition_test.cpp
    objects/interactive/Bullet_test.cpp
    objects/interactive/BulletAim_test.cpp
    objects/interactive/BulletExplosion_test.cpp
    objects/interactive/Crystal_test.cpp
    objects/interactive/Heart_test.cpp
    objects/interactive/Platform_test.cpp
    objects/interactive/PlatformCollision_test.cpp
    objects/interactive/Tree_test.cpp

    objects/interfaces/Movable_test.cpp

    objects/units/EnemyCollision_test.cpp
    objects/units/PlayerFiring_test.cpp
    objects/units/Shooter_test.cpp
    objects/units/ShooterPlatformCollision_test.cpp
    objects/units/ShooterSprite_test.cpp
    objects/units/Unit_test.cpp

    ui/Dialogue_test.cpp
)

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/tests)
target_link_libraries(tests shooter_lib GTest::gtest_main sfml-graphics sfml-window sfml-system sfml-audio)

include(GoogleTest)
gtest_discover_tests(tests)
