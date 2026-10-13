#include <gtest/gtest.h>
#include <vector>

#include "LevelGenerator.h"

static GeneratedObject makeFlagTestBlock(float x, float y, int width) {
    GeneratedObject block;
    block.x = x;
    block.y = y;
    block.width = width;
    block.height = 25;
    block.type = "GroundBlock";
    return block;
}

// Three ground blocks; the block at y=1000 is the highest one.
static std::vector<GeneratedObject> flagTestBlocks() {
    std::vector<GeneratedObject> objects;
    objects.push_back(makeFlagTestBlock(100, 2000, 300));
    objects.push_back(makeFlagTestBlock(500, 1000, 300));
    objects.push_back(makeFlagTestBlock(900, 1500, 300));
    return objects;
}

static const GeneratedObject* findFlag(const std::vector<GeneratedObject>& objects) {
    for (const auto& object : objects) {
        if (object.type == "Flag") {
            return &object;
        }
    }
    return nullptr;
}

static int countFlags(const std::vector<GeneratedObject>& objects) {
    int count = 0;
    for (const auto& object : objects) {
        if (object.type == "Flag") {
            count++;
        }
    }
    return count;
}

TEST(FlagTest, FlagPlacedOnHighestPlatform) {
    std::vector<GeneratedObject> objects = flagTestBlocks();
    LevelGenerator generator(nullptr);

    generator.generateFlag(objects);

    const GeneratedObject* flag = findFlag(objects);
    ASSERT_NE(flag, nullptr);
    EXPECT_FLOAT_EQ(flag->y, 1000.f - LevelGenerator::FLAG_SIZE);
    EXPECT_FLOAT_EQ(flag->x, 625.f);
}

TEST(FlagTest, FlagRestsOnPlatformTop) {
    std::vector<GeneratedObject> objects = flagTestBlocks();
    LevelGenerator generator(nullptr);

    generator.generateFlag(objects);

    const GeneratedObject* flag = findFlag(objects);
    ASSERT_NE(flag, nullptr);
    EXPECT_EQ(flag->width, LevelGenerator::FLAG_SIZE);
    EXPECT_EQ(flag->height, LevelGenerator::FLAG_SIZE);
    EXPECT_FLOAT_EQ(flag->y, 1000.f - LevelGenerator::FLAG_SIZE);
}

TEST(FlagTest, ExactlyOneFlagGenerated) {
    std::vector<GeneratedObject> objects = flagTestBlocks();
    LevelGenerator generator(nullptr);

    generator.generateFlag(objects);

    EXPECT_EQ(countFlags(objects), 1);
}
