#include <gtest/gtest.h>
#include "objects/units/Unit.h"

static GameObjects* createDummyGameObjects() {
    return reinterpret_cast<GameObjects*>(0x1);
}

TEST(UnitTest, DefaultHealth) {
    Unit u(createDummyGameObjects(), 0, 0, 10, 10);
    EXPECT_EQ(u.getHealth(), 3);
}

TEST(UnitTest, DefaultAmmo) {
    Unit u(createDummyGameObjects(), 0, 0, 10, 10);
    EXPECT_EQ(u.getAmmo(), 10);
}

TEST(UnitTest, DefaultCrystals) {
    Unit u(createDummyGameObjects(), 0, 0, 10, 10);
    EXPECT_EQ(u.getCrystals(), 0);
}

TEST(UnitTest, IsDeadDefaultFalse) {
    Unit u(createDummyGameObjects(), 0, 0, 10, 10);
    EXPECT_FALSE(u.isDead());
}

TEST(UnitTest, IsPlayerDefaultFalse) {
    Unit u(createDummyGameObjects(), 0, 0, 10, 10);
    EXPECT_FALSE(u.isPlayer());
}

TEST(UnitTest, IsMustBeDeletedDefaultFalse) {
    Unit u(createDummyGameObjects(), 0, 0, 10, 10);
    EXPECT_FALSE(u.isMustBeDeleted());
}

TEST(UnitTest, SetDeadAndIsDead) {
    Unit u(createDummyGameObjects(), 0, 0, 10, 10);
    u.setDead(true);
    EXPECT_TRUE(u.isDead());
    u.setDead(false);
    EXPECT_FALSE(u.isDead());
}

TEST(UnitTest, SetMainAndIsPlayer) {
    Unit u(createDummyGameObjects(), 0, 0, 10, 10);
    u.setMain(true);
    EXPECT_TRUE(u.isPlayer());
    u.setMain(false);
    EXPECT_FALSE(u.isPlayer());
}

TEST(UnitTest, DecreaseHealthReducesByOne) {
    Unit u(createDummyGameObjects(), 0, 0, 10, 10);
    int before = u.getHealth();
    u.decreaseHealth();
    EXPECT_EQ(u.getHealth(), before - 1);
}

TEST(UnitTest, DecreaseHealthMultipleTimes) {
    Unit u(createDummyGameObjects(), 0, 0, 10, 10);
    u.decreaseHealth();
    u.decreaseHealth();
    u.decreaseHealth();
    EXPECT_EQ(u.getHealth(), 0);
}

TEST(UnitTest, CanJumpDefaultTrue) {
    Unit u(createDummyGameObjects(), 0, 0, 10, 10);
    EXPECT_TRUE(u.canJump());
}

TEST(UnitTest, IsFallingDefaultFalse) {
    Unit u(createDummyGameObjects(), 0, 0, 10, 10);
    EXPECT_FALSE(u.isFalling());
}

TEST(UnitTest, IsJumpingDefaultFalse) {
    Unit u(createDummyGameObjects(), 0, 0, 10, 10);
    EXPECT_FALSE(u.isJumping());
}

TEST(UnitTest, GetCurrentJumpHeightDefaultZero) {
    Unit u(createDummyGameObjects(), 0, 0, 10, 10);
    EXPECT_FLOAT_EQ(u.getCurrentJumpHeight(), 0.f);
}

TEST(UnitTest, GetVelocityHorizontalDefaultZero) {
    Unit u(createDummyGameObjects(), 0, 0, 10, 10);
    EXPECT_FLOAT_EQ(u.getVelocityHorizontal(), 0.f);
}

TEST(UnitTest, SetVelocityUpdatesValue) {
    Unit u(createDummyGameObjects(), 0, 0, 10, 10);
    u.setVelocity(3.0f);
}
