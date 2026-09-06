#include <gtest/gtest.h>
#include "CObject.h"
#include "Event.h"

TEST(GameObjectsTest, CObjectParameterizedConstructor) {
    CObject obj(5, 10, 100, 200);
    EXPECT_EQ(obj.getX(), 5);
    EXPECT_EQ(obj.getY(), 10);
    EXPECT_EQ(obj.getWidth(), 100);
    EXPECT_EQ(obj.getHeight(), 200);
}

TEST(GameObjectsTest, CObjectSetCoords) {
    CObject obj(0, 0, 10, 10);
    obj.setCoords(50, 60);
    EXPECT_FLOAT_EQ(obj.getX(), 50);
    EXPECT_FLOAT_EQ(obj.getY(), 60);
}

TEST(GameObjectsTest, CObjectCollidePointInside) {
    CObject obj(0, 0, 100, 100);
    EXPECT_TRUE(obj.collidePoint(50, 50));
    EXPECT_TRUE(obj.collidePoint(1, 1));
    EXPECT_TRUE(obj.collidePoint(98, 98));
}

TEST(GameObjectsTest, CObjectCollidePointOutside) {
    CObject obj(0, 0, 100, 100);
    EXPECT_FALSE(obj.collidePoint(-1, -1));
    EXPECT_FALSE(obj.collidePoint(100, 100));
}

TEST(GameObjectsTest, CObjectCollideRectTouching) {
    CObject obj1(0, 0, 50, 50);
    CObject obj2(50, 0, 50, 50);
    EXPECT_FALSE(obj1.collideRect(&obj2));
}

TEST(GameObjectsTest, CObjectCollideRectPartialOverlap) {
    CObject obj1(0, 0, 60, 60);
    CObject obj2(30, 30, 60, 60);
    EXPECT_TRUE(obj1.collideRect(&obj2));
}

TEST(GameObjectsTest, CObjectCollideRectFullyInside) {
    CObject obj1(0, 0, 100, 100);
    CObject obj2(25, 25, 50, 50);
    EXPECT_TRUE(obj1.collideRect(&obj2));
}

TEST(GameObjectsTest, CObjectUpdateDoesNotCrash) {
    CObject obj(0, 0, 10, 10);
    Event event;
    obj.update(event);
}
