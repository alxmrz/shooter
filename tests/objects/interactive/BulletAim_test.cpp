#include <gtest/gtest.h>
#include <cmath>
#include "fixtures/GameObjectsTestFixture.h"
#include "objects/interactive/Bullet.h"

namespace {

const float BULLET_SPEED = 8.f;

float speedOf(Bullet* bullet) {
    return std::sqrt(
        bullet->getVelocityX() * bullet->getVelocityX() +
        bullet->getVelocityY() * bullet->getVelocityY()
    );
}

}  // namespace

TEST_F(GameObjectsFixture, BulletFlightStepIsEightAlongDirection) {
    Bullet* bullet = spawnBullet(100, 100);
    bullet->setFlightDirection(sf::Vector2f(1.f, 1.f));

    bullet->update(Event());

    EXPECT_NEAR(speedOf(bullet), BULLET_SPEED, 0.001f);
    EXPECT_NEAR(bullet->getX() - 100.f, bullet->getVelocityX(), 0.001f);
    EXPECT_NEAR(bullet->getY() - 100.f, bullet->getVelocityY(), 0.001f);
}

TEST_F(GameObjectsFixture, BulletFliesTowardAimUpDown) {
    Bullet* bullet = spawnBullet(100, 100);
    bullet->setFlightDirection(sf::Vector2f(0.f, -1.f));

    bullet->update(Event());

    EXPECT_FLOAT_EQ(bullet->getX(), 100.f);
    EXPECT_FLOAT_EQ(bullet->getY(), 92.f);
}

TEST_F(GameObjectsFixture, BulletFliesTowardAimBackward) {
    Bullet* bullet = spawnBullet(100, 100);
    bullet->setFlightDirection(sf::Vector2f(-1.f, 0.f));

    bullet->update(Event());

    EXPECT_FLOAT_EQ(bullet->getX(), 92.f);
    EXPECT_FLOAT_EQ(bullet->getY(), 100.f);
}

TEST_F(GameObjectsFixture, BulletSpriteRotatesWithDirection) {
    Bullet* bullet = spawnBullet(100, 100);
    bullet->setFlightDirection(sf::Vector2f(0.f, -1.f));

    EXPECT_NEAR(bullet->getRotation(), -90.f, 0.01f);
}

TEST_F(GameObjectsFixture, BulletMaxDistanceIndependentOfDirection) {
    Bullet* bullet = spawnBullet(100, 100);
    bullet->setFlightDirection(sf::Vector2f(1.f, -1.f));

    driveUpdates(bullet, 63);

    EXPECT_TRUE(bullet->isExploding());
}

TEST_F(GameObjectsFixture, BulletHorizontalCompatibility) {
    Bullet* bullet = spawnBullet(100, 100);
    bullet->setDirection("right");

    bullet->update(Event());

    EXPECT_FLOAT_EQ(bullet->getX(), 108.f);
    EXPECT_FLOAT_EQ(bullet->getY(), 100.f);
}
