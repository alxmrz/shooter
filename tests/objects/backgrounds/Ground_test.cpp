#include <gtest/gtest.h>
#include "objects/backgrounds/Ground.h"

TEST(GroundTest, ParameterizedConstructorSetsFields) {
    Ground g(10, 20, 100, 50);
    EXPECT_FLOAT_EQ(g.getX(), 10);
    EXPECT_FLOAT_EQ(g.getY(), 20);
    EXPECT_EQ(g.getWidth(), 100);
    EXPECT_EQ(g.getHeight(), 50);
}

TEST(GroundTest, NegativeCoordinates) {
    Ground g(-10, -20, 50, 50);
    EXPECT_FLOAT_EQ(g.getX(), -10);
    EXPECT_FLOAT_EQ(g.getY(), -20);
}

TEST(GroundTest, ZeroSize) {
    Ground g(0, 0, 0, 0);
    EXPECT_EQ(g.getWidth(), 0);
    EXPECT_EQ(g.getHeight(), 0);
}

TEST(GroundTest, CollidePointInside) {
    Ground g(0, 0, 100, 100);
    EXPECT_TRUE(g.collidePoint(50, 50));
}

TEST(GroundTest, CollidePointOutside) {
    Ground g(0, 0, 100, 100);
    EXPECT_FALSE(g.collidePoint(200, 200));
}

TEST(GroundTest, SetCoordsUpdatesPosition) {
    Ground g(0, 0, 10, 10);
    g.setCoords(100, 200);
    EXPECT_FLOAT_EQ(g.getX(), 100);
    EXPECT_FLOAT_EQ(g.getY(), 200);
}
