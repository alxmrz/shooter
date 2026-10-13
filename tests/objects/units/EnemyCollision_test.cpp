#include <gtest/gtest.h>
#include "fixtures/GameObjectsTestFixture.h"

// Enemies must collide with ground blocks exactly like the player does.
// DSL: placeGroundBlock / spawnEnemy build the world; each test asserts one case.

TEST_F(GameObjectsFixture, EnemyLandsOnGroundBlock) {
    GroundBlock* block = placeGroundBlock(100, 200);
    Shooter* enemy = spawnEnemy(100, 100);

    enemy->move(0.f, 40.f);

    EXPECT_FLOAT_EQ(enemy->getY(), block->getY() - ACTOR_H);
}

TEST_F(GameObjectsFixture, EnemyDoesNotFallToBottom) {
    GroundBlock* block = placeGroundBlock(100, 200);
    Shooter* enemy = spawnEnemy(100, 100);

    for (int frame = 0; frame < 100; ++frame) {
        enemy->move(0.f, 10.f);
    }

    EXPECT_FLOAT_EQ(enemy->getY(), block->getY() - ACTOR_H);
}

TEST_F(GameObjectsFixture, EnemyBlockedFromSide) {
    placeGroundBlock(140, 100);
    Shooter* enemy = spawnEnemy(130, 110);
    const float startX = enemy->getX();
    const float startY = enemy->getY();

    enemy->move(10.f, 0.f);

    EXPECT_FLOAT_EQ(enemy->getX(), startX);
    EXPECT_FLOAT_EQ(enemy->getY(), startY);
}

TEST_F(GameObjectsFixture, EnemyBlockedFromBelow) {
    placeGroundBlock(100, 100);
    Shooter* enemy = spawnEnemy(100, 115);
    const float startY = enemy->getY();

    enemy->move(0.f, -10.f);

    EXPECT_FLOAT_EQ(enemy->getY(), startY);
}
