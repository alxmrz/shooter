#include <gtest/gtest.h>
#include "objects/backgrounds/GroundBlock.h"

class GroundBlockTest : public ::testing::Test {
protected:
    void SetUp() override {
    }
};

TEST_F(GroundBlockTest, ParameterizedConstructorSetsFields) {
    GroundBlock block(100, 200, 50, 25);
    
    EXPECT_EQ(block.getX(), 100);
    EXPECT_EQ(block.getY(), 200);
    EXPECT_EQ(block.getWidth(), 50);
    EXPECT_EQ(block.getHeight(), 25);
}

TEST_F(GroundBlockTest, NegativeCoordinates) {
    GroundBlock block(-50, -100, 50, 25);
    
    EXPECT_EQ(block.getX(), -50);
    EXPECT_EQ(block.getY(), -100);
}

TEST_F(GroundBlockTest, ZeroSize) {
    GroundBlock block(0, 0, 0, 0);
    
    EXPECT_EQ(block.getX(), 0);
    EXPECT_EQ(block.getY(), 0);
    EXPECT_EQ(block.getWidth(), 0);
    EXPECT_EQ(block.getHeight(), 0);
}

TEST_F(GroundBlockTest, CollidePointInside) {
    GroundBlock block(100, 100, 50, 25);
    
    EXPECT_TRUE(block.collidePoint(125, 112));
}

TEST_F(GroundBlockTest, CollidePointOutside) {
    GroundBlock block(100, 100, 50, 25);
    
    EXPECT_FALSE(block.collidePoint(50, 50));
}

TEST_F(GroundBlockTest, SetCoordsUpdatesPosition) {
    GroundBlock block(100, 100, 50, 25);
    block.setCoords(200, 200);
    
    EXPECT_EQ(block.getX(), 200);
    EXPECT_EQ(block.getY(), 200);
}
