#include <gtest/gtest.h>
#include "objects/interactive/Ammunition.h"

TEST(AmmunitionTest, ParameterizedConstructorSetsFields) {
    Ammunition a(50, 60, 30, 30);
    EXPECT_FLOAT_EQ(a.getX(), 50);
    EXPECT_FLOAT_EQ(a.getY(), 60);
    EXPECT_EQ(a.getWidth(), 30);
    EXPECT_EQ(a.getHeight(), 30);
}

TEST(AmmunitionTest, CollideRectWithOverlappingAmmunition) {
    Ammunition a1(0, 0, 50, 50);
    Ammunition a2(25, 25, 50, 50);
    EXPECT_TRUE(a1.collideRect(&a2));
}

TEST(AmmunitionTest, CollideRectWithDistantAmmunition) {
    Ammunition a1(0, 0, 50, 50);
    Ammunition a2(1000, 1000, 50, 50);
    EXPECT_FALSE(a1.collideRect(&a2));
}

TEST(AmmunitionTest, CollidePointInside) {
    Ammunition a(0, 0, 100, 100);
    EXPECT_TRUE(a.collidePoint(50, 50));
}

TEST(AmmunitionTest, CollidePointOutside) {
    Ammunition a(0, 0, 100, 100);
    EXPECT_FALSE(a.collidePoint(200, 200));
}
