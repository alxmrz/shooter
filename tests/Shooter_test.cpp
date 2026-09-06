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
