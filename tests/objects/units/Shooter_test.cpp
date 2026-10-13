#include <gtest/gtest.h>
#include "objects/units/Shooter.h"

static GameObjects* createDummyGameObjects() {
    return reinterpret_cast<GameObjects*>(0x1);
}

TEST(ShooterTest, DefaultHealth) {
    Shooter s(createDummyGameObjects(), 0, 0, 10, 10);
    EXPECT_EQ(s.getHealth(), 3);
}

TEST(ShooterTest, DefaultAmmo) {
    Shooter s(createDummyGameObjects(), 0, 0, 10, 10);
    EXPECT_EQ(s.getAmmo(), 10);
}

TEST(ShooterTest, DefaultCrystals) {
    Shooter s(createDummyGameObjects(), 0, 0, 10, 10);
    EXPECT_EQ(s.getCrystals(), 0);
}

TEST(ShooterTest, IsDeadDefaultFalse) {
    Shooter s(createDummyGameObjects(), 0, 0, 10, 10);
    EXPECT_FALSE(s.isDead());
}

TEST(ShooterTest, IsPlayerDefaultFalse) {
    Shooter s(createDummyGameObjects(), 0, 0, 10, 10);
    EXPECT_FALSE(s.isPlayer());
}

TEST(ShooterTest, SetDeadAndIsDead) {
    Shooter s(createDummyGameObjects(), 0, 0, 10, 10);
    s.setDead(true);
    EXPECT_TRUE(s.isDead());
}

TEST(ShooterTest, SetMainAndIsPlayer) {
    Shooter s(createDummyGameObjects(), 0, 0, 10, 10);
    s.setMain(true);
    EXPECT_TRUE(s.isPlayer());
}

TEST(ShooterTest, OnSecondJumpDoesNotCrash) {
    Shooter s(createDummyGameObjects(), 0, 0, 10, 10);
    s.onSecondJump();
}

TEST(ShooterTest, DecreaseHealth) {
    Shooter s(createDummyGameObjects(), 0, 0, 10, 10);
    s.decreaseHealth();
    EXPECT_EQ(s.getHealth(), 2);
}

TEST(ShooterTest, DecreaseHealthToZero) {
    Shooter s(createDummyGameObjects(), 0, 0, 10, 10);
    s.decreaseHealth();
    s.decreaseHealth();
    s.decreaseHealth();
    EXPECT_EQ(s.getHealth(), 0);
}

TEST(ShooterTest, MainSpriteOriginIsCenter) {
    Shooter s(createDummyGameObjects(), 0, 0, 50, 50);
    sf::Sprite sprite;
    s.setMainSprite(&sprite);

    sf::Sprite* drawn = dynamic_cast<sf::Sprite*>(s.getDrawForm());
    ASSERT_NE(drawn, nullptr);
    EXPECT_FLOAT_EQ(drawn->getOrigin().x, 25.f);
    EXPECT_FLOAT_EQ(drawn->getOrigin().y, 25.f);
}

TEST(ShooterTest, ShiftSpritePositionsKeepsVisualTopLeft) {
    Shooter s(createDummyGameObjects(), 12, 34, 50, 50);
    sf::Sprite sprite;
    sf::Sprite explosionSprite;
    s.setMainSprite(&sprite);
    s.setExplosionSprite(&explosionSprite);
    s.shiftSpritePositions();

    sf::Sprite* drawn = dynamic_cast<sf::Sprite*>(s.getDrawForm());
    ASSERT_NE(drawn, nullptr);
    EXPECT_FLOAT_EQ(drawn->getOrigin().x, 25.f);
    EXPECT_FLOAT_EQ(drawn->getOrigin().y, 25.f);
    EXPECT_FLOAT_EQ(drawn->getPosition().x, s.getX() + drawn->getOrigin().x);
    EXPECT_FLOAT_EQ(drawn->getPosition().y, s.getY() + drawn->getOrigin().y);
}

TEST(ShooterTest, RotationCompletesFullTurnAndStops) {
    Shooter s(createDummyGameObjects(), 0, 0, 50, 50);
    sf::Sprite sprite;
    s.setMainSprite(&sprite);

    s.onSecondJump();
    s.applyRotation(0.25f);
    EXPECT_NEAR(sprite.getRotation(), 180.f, 0.01f);

    s.applyRotation(0.25f);
    EXPECT_FLOAT_EQ(sprite.getRotation(), 0.f);

    s.applyRotation(1.0f);
    EXPECT_FLOAT_EQ(sprite.getRotation(), 0.f);
}
