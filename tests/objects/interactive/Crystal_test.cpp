#include <gtest/gtest.h>
#include "objects/interactive/Crystal.h"

TEST(CrystalTest, ParameterizedConstructorSetsFields) {
    Crystal c(100, 200, 50, 50);
    EXPECT_FLOAT_EQ(c.getX(), 100);
    EXPECT_FLOAT_EQ(c.getY(), 200);
    EXPECT_EQ(c.getWidth(), 50);
    EXPECT_EQ(c.getHeight(), 50);
}

TEST(CrystalTest, CollideRectWithOverlappingCrystal) {
    Crystal c1(0, 0, 50, 50);
    Crystal c2(25, 25, 50, 50);
    EXPECT_TRUE(c1.collideRect(&c2));
}

TEST(CrystalTest, CollideRectWithDistantCrystal) {
    Crystal c1(0, 0, 50, 50);
    Crystal c2(1000, 1000, 50, 50);
    EXPECT_FALSE(c1.collideRect(&c2));
}

TEST(CrystalTest, CollideRectTouching) {
    Crystal c1(0, 0, 50, 50);
    Crystal c2(50, 0, 50, 50);
    EXPECT_FALSE(c1.collideRect(&c2));
}

TEST(CrystalTest, SetCoordsUpdatesPosition) {
    Crystal c(0, 0, 10, 10);
    c.setCoords(77, 88);
    EXPECT_FLOAT_EQ(c.getX(), 77);
    EXPECT_FLOAT_EQ(c.getY(), 88);
}
