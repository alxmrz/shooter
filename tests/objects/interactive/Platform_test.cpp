#include <gtest/gtest.h>
#include "objects/interactive/Platform.h"

TEST(PlatformTest, ParameterizedConstructorSetsFields) {
    Platform p(0, 100, 200, 25);
    EXPECT_FLOAT_EQ(p.getX(), 0);
    EXPECT_FLOAT_EQ(p.getY(), 100);
    EXPECT_EQ(p.getWidth(), 200);
    EXPECT_EQ(p.getHeight(), 25);
}

TEST(PlatformTest, GetDrawFormReturnsNonNull) {
    Platform p(0, 0, 100, 25);
    EXPECT_NE(p.getDrawForm(), nullptr);
}

TEST(PlatformTest, CollideRectWithOverlappingPlatform) {
    Platform p1(0, 0, 100, 25);
    Platform p2(50, 10, 100, 25);
    EXPECT_TRUE(p1.collideRect(&p2));
}

TEST(PlatformTest, CollideRectWithDistantPlatform) {
    Platform p1(0, 0, 100, 25);
    Platform p2(500, 500, 100, 25);
    EXPECT_FALSE(p1.collideRect(&p2));
}

TEST(PlatformTest, CollidePointInside) {
    Platform p(0, 0, 100, 25);
    EXPECT_TRUE(p.collidePoint(50, 10));
}

TEST(PlatformTest, CollidePointOutside) {
    Platform p(0, 0, 100, 25);
    EXPECT_FALSE(p.collidePoint(200, 200));
}
