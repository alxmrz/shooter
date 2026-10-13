#include <gtest/gtest.h>
#include "objects/interfaces/Movable.h"

TEST(MovableTest, DefaultConstructorInitialValues) {
    Movable m;
    EXPECT_FALSE(m.isMoving());
    EXPECT_FLOAT_EQ(m.getVelocity(), 0.f);
    EXPECT_FLOAT_EQ(m.getAcceleration(), 0.3f);
    EXPECT_FLOAT_EQ(m.getMaxVelocity(), 5.f);
}

TEST(MovableTest, IsMovingDefaultFalse) {
    Movable m;
    EXPECT_FALSE(m.isMoving());
}

TEST(MovableTest, StopMovingResetsState) {
    Movable m;
    m.stopMoving();
    EXPECT_FALSE(m.isMoving());
    EXPECT_FLOAT_EQ(m.getVelocity(), 0.f);
}

TEST(MovableTest, CalcVelocityRightIncreasesVelocity) {
    Movable m;
    m.calcVelocity();
    EXPECT_GT(m.getVelocity(), 0.f);
}

TEST(MovableTest, CalcVelocityApproachesMaxVelocity) {
    Movable m;
    for (int i = 0; i < 100; i++) {
        m.calcVelocity();
    }
    EXPECT_GE(m.getVelocity(), m.getMaxVelocity());
}

TEST(MovableTest, GettersReturnCorrectValues) {
    Movable m;
    EXPECT_FLOAT_EQ(m.getAcceleration(), 0.3f);
    EXPECT_FLOAT_EQ(m.getMaxVelocity(), 5.f);
    EXPECT_FLOAT_EQ(m.getVelocity(), 0.f);
}
