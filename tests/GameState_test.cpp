#include <gtest/gtest.h>
#include "Event.h"
#include "CObject.h"

TEST(EventTest, DefaultConstructorCreatesValidEvent) {
    Event event;
    EXPECT_FALSE(event.HasType(Event::Type::Closed));
    EXPECT_FALSE(event.HasType(Event::Type::KeyPressed));
}

TEST(EventTest, TypeConstructorSetsType) {
    Event event(static_cast<int>(Event::Type::Closed));
    EXPECT_TRUE(event.HasType(Event::Type::Closed));
}

TEST(EventTest, HasTypeReturnsFalseForUnsetTypes) {
    Event event(static_cast<int>(Event::Type::KeyPressed));
    EXPECT_FALSE(event.HasType(Event::Type::Closed));
}

TEST(CObjectTest, ParameterizedConstructorSetsFields) {
    CObject obj(10, 20, 100, 50);
    EXPECT_EQ(obj.getX(), 10);
    EXPECT_EQ(obj.getY(), 20);
    EXPECT_EQ(obj.getWidth(), 100);
    EXPECT_EQ(obj.getHeight(), 50);
}

TEST(CObjectTest, SetCoordsUpdatesPosition) {
    CObject obj(0, 0, 10, 10);
    obj.setCoords(100, 200);
    EXPECT_EQ(obj.getX(), 100);
    EXPECT_EQ(obj.getY(), 200);
}

TEST(CObjectTest, CollidePointInsideReturnsTrue) {
    CObject obj(10, 10, 50, 50);
    EXPECT_TRUE(obj.collidePoint(25, 25));
}

TEST(CObjectTest, CollidePointOutsideReturnsFalse) {
    CObject obj(10, 10, 50, 50);
    EXPECT_FALSE(obj.collidePoint(0, 0));
    EXPECT_FALSE(obj.collidePoint(100, 100));
}

TEST(CObjectTest, CollideRectOverlappingReturnsTrue) {
    CObject obj1(0, 0, 50, 50);
    CObject obj2(25, 25, 50, 50);
    EXPECT_TRUE(obj1.collideRect(&obj2));
}

TEST(CObjectTest, CollideRectNonOverlappingReturnsFalse) {
    CObject obj1(0, 0, 50, 50);
    CObject obj2(100, 100, 50, 50);
    EXPECT_FALSE(obj1.collideRect(&obj2));
}

TEST(CObjectTest, GetBorderPointsReturnsEightPoints) {
    CObject obj(10, 20, 30, 40);
    auto points = obj.getBorderPoints();
    ASSERT_EQ(points.size(), 8u);
    EXPECT_EQ(points[0][0], 10);
    EXPECT_EQ(points[0][1], 20);
}

TEST(CObjectTest, UpdateDoesNotCrash) {
    CObject obj(0, 0, 10, 10);
    Event event;
    obj.update(event);
}
