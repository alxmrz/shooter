#include <gtest/gtest.h>
#include "fixtures/GameObjectsTestFixture.h"

namespace {

const int EXPLOSION_FRAME_DELAY = 5;
const int EXPLOSION_FRAME_COUNT = 6;
const sf::IntRect FRAME_ONE(80, 0, 80, 75);

sf::IntRect explosionFrameOf(Bullet* bullet) {
    return bullet->getExplosionSprite()->getTextureRect();
}

}  // namespace

TEST_F(GameObjectsFixture, BulletSpeedStepMovesByEight) {
    Bullet* bullet = spawnBullet(100, 100);
    bullet->setDirection("right");

    bullet->update(Event());

    EXPECT_FLOAT_EQ(bullet->getX(), 108.f);
}

TEST_F(GameObjectsFixture, BulletExplodesOnBorderCollision) {
    Bullet* bullet = spawnBullet(100, 100);
    bullet->setDirection("right");
    placeBorder(105, 100, 50, 10);

    bullet->update(Event());

    EXPECT_TRUE(bullet->isExploding());
}

TEST_F(GameObjectsFixture, BulletExplodesOnUnitHit) {
    Shooter* player = spawnPlayer(0, 0);
    Bullet* bullet = spawnBullet(100, 100, player);
    bullet->setDirection("right");
    spawnEnemy(105, 100);

    bullet->update(Event());

    EXPECT_TRUE(bullet->isExploding());
}

TEST_F(GameObjectsFixture, BulletExplodesAtMaxDistance) {
    Bullet* bullet = spawnBullet(100, 100);
    bullet->setDirection("right");

    driveUpdates(bullet, 63);

    EXPECT_TRUE(bullet->isExploding());
}

TEST_F(GameObjectsFixture, ExplosionAnimationAdvancesFrames) {
    Bullet* bullet = spawnBullet(100, 100);
    bullet->setDirection("right");
    placeBorder(105, 100, 50, 10);
    bullet->update(Event());

    driveUpdates(bullet, EXPLOSION_FRAME_DELAY);

    EXPECT_EQ(explosionFrameOf(bullet), FRAME_ONE);
}

TEST_F(GameObjectsFixture, BulletRemovedAfterExplosionCompletes) {
    Bullet* bullet = spawnBullet(100, 100);
    bullet->setDirection("right");
    placeBorder(105, 100, 50, 10);
    bullet->update(Event());

    driveUpdates(bullet, EXPLOSION_FRAME_DELAY * EXPLOSION_FRAME_COUNT);

    EXPECT_FALSE(containsObject(bullet));
}

TEST_F(GameObjectsFixture, ExplosionTransitionHappensOnce) {
    Bullet* bullet = spawnBullet(100, 100);
    bullet->setDirection("right");
    placeBorder(105, 100, 50, 10);
    bullet->update(Event());

    driveUpdates(bullet, EXPLOSION_FRAME_DELAY * 2);

    EXPECT_EQ(bullet->getExplosionSoundPlays(), 1);
}
