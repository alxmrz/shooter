#include <gtest/gtest.h>
#include "fixtures/GameObjectsTestFixture.h"

TEST_F(GameObjectsFixture, PlayerAttackWithAimSpawnsAngledBullet) {
    RecordingShooter* player = spawnRecordingPlayer(100, 100);
    player->setAimTarget(200, 50);

    player->attack();

    ASSERT_EQ(player->spawned.size(), 1u);
    EXPECT_GT(player->spawned[0]->getVelocityX(), 0.f);
    EXPECT_LT(player->spawned[0]->getVelocityY(), 0.f);
}

TEST_F(GameObjectsFixture, PlayerAttackWithoutAimFallsBackToFacing) {
    RecordingShooter* player = spawnRecordingPlayer(100, 100);

    player->attack();

    ASSERT_EQ(player->spawned.size(), 1u);
    EXPECT_FLOAT_EQ(player->spawned[0]->getVelocityX(), 8.f);
    EXPECT_FLOAT_EQ(player->spawned[0]->getVelocityY(), 0.f);
}

TEST_F(GameObjectsFixture, PlayerAttackCoincidentAimFallsBackToFacing) {
    RecordingShooter* player = spawnRecordingPlayer(100, 100);
    player->setAimTarget(125, 140);

    player->attack();

    ASSERT_EQ(player->spawned.size(), 1u);
    EXPECT_FLOAT_EQ(player->spawned[0]->getVelocityX(), 8.f);
    EXPECT_FLOAT_EQ(player->spawned[0]->getVelocityY(), 0.f);
}
