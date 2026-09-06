#include <gtest/gtest.h>
#include "objects/interactive/Heart.h"

TEST(HeartTest, ParameterizedConstructorSetsFields) {
    Heart h(10, 20, 40, 40);
    EXPECT_FLOAT_EQ(h.getX(), 10);
    EXPECT_FLOAT_EQ(h.getY(), 20);
    EXPECT_EQ(h.getWidth(), 40);
    EXPECT_EQ(h.getHeight(), 40);
}

TEST(HeartTest, CollideRectWithOverlappingHeart) {
    Heart h1(0, 0, 50, 50);
    Heart h2(25, 25, 50, 50);
    EXPECT_TRUE(h1.collideRect(&h2));
}

TEST(HeartTest, CollideRectWithDistantHeart) {
    Heart h1(0, 0, 50, 50);
    Heart h2(500, 500, 50, 50);
    EXPECT_FALSE(h1.collideRect(&h2));
}

TEST(HeartTest, CollidePointInside) {
    Heart h(10, 10, 50, 50);
    EXPECT_TRUE(h.collidePoint(25, 25));
}

TEST(HeartTest, CollidePointOutside) {
    Heart h(10, 10, 50, 50);
    EXPECT_FALSE(h.collidePoint(0, 0));
}
