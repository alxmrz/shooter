#include <gtest/gtest.h>
#include "CObject.h"
#include "Event.h"
#include "objects/interactive/Crystal.h"
#include "objects/interactive/Ammunition.h"
#include "objects/interactive/Heart.h"
#include "objects/interactive/Platform.h"

TEST(FabricTest, CrystalCreation) {
    Crystal crystal(100, 200, 50, 50);
    EXPECT_FLOAT_EQ(crystal.getX(), 100);
    EXPECT_FLOAT_EQ(crystal.getY(), 200);
    EXPECT_EQ(crystal.getWidth(), 50);
    EXPECT_EQ(crystal.getHeight(), 50);
}

TEST(FabricTest, AmmunitionCreation) {
    Ammunition ammo(50, 60, 30, 30);
    EXPECT_FLOAT_EQ(ammo.getX(), 50);
    EXPECT_FLOAT_EQ(ammo.getY(), 60);
    EXPECT_EQ(ammo.getWidth(), 30);
    EXPECT_EQ(ammo.getHeight(), 30);
}

TEST(FabricTest, HeartCreation) {
    Heart heart(10, 20, 40, 40);
    EXPECT_FLOAT_EQ(heart.getX(), 10);
    EXPECT_FLOAT_EQ(heart.getY(), 20);
    EXPECT_EQ(heart.getWidth(), 40);
    EXPECT_EQ(heart.getHeight(), 40);
}

TEST(FabricTest, PlatformCreation) {
    Platform platform(0, 100, 200, 25);
    EXPECT_FLOAT_EQ(platform.getX(), 0);
    EXPECT_FLOAT_EQ(platform.getY(), 100);
    EXPECT_EQ(platform.getWidth(), 200);
    EXPECT_EQ(platform.getHeight(), 25);
}

TEST(FabricTest, CObjectCreation) {
    CObject obj(15, 25, 35, 45);
    EXPECT_FLOAT_EQ(obj.getX(), 15);
    EXPECT_FLOAT_EQ(obj.getY(), 25);
    EXPECT_EQ(obj.getWidth(), 35);
    EXPECT_EQ(obj.getHeight(), 45);
}

TEST(FabricTest, MultipleObjectsCollideCorrectly) {
    Crystal c1(0, 0, 50, 50);
    Crystal c2(25, 25, 50, 50);
    EXPECT_TRUE(c1.collideRect(&c2));
}

TEST(FabricTest, ObjectsDoNotCollideWhenFarApart) {
    Crystal c1(0, 0, 50, 50);
    Ammunition a1(1000, 1000, 50, 50);
    EXPECT_FALSE(c1.collideRect(&a1));
}
