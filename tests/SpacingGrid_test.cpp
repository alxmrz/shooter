#include <gtest/gtest.h>
#include "SpacingGrid.h"

namespace {
const int CELL_SIZE = 100;
const float HALF_GAP = 50.f;
}

TEST(SpacingGridTest, AllowsDistantBoxes) {
    SpacingGrid grid(CELL_SIZE, HALF_GAP);

    SpacingBox first = {0.f, 0.f, 100, 25};
    SpacingBox second = {1000.f, 1000.f, 100, 25};

    ASSERT_TRUE(grid.canPlace(first));
    grid.insert(first);

    EXPECT_TRUE(grid.canPlace(second));
}

TEST(SpacingGridTest, RejectsBoxesCloserThanTheGap) {
    SpacingGrid grid(CELL_SIZE, HALF_GAP);

    SpacingBox first = {0.f, 0.f, 100, 25};
    SpacingBox tooClose = {150.f, 0.f, 100, 25}; // actual horizontal gap = 50 < 100

    ASSERT_TRUE(grid.canPlace(first));
    grid.insert(first);

    EXPECT_FALSE(grid.canPlace(tooClose));
}

TEST(SpacingGridTest, AllowsBoxesWithExactGap) {
    SpacingGrid grid(CELL_SIZE, HALF_GAP);

    SpacingBox first = {0.f, 0.f, 100, 25};
    SpacingBox exactlyGap = {200.f, 0.f, 100, 25}; // actual horizontal gap = 100

    ASSERT_TRUE(grid.canPlace(first));
    grid.insert(first);

    EXPECT_TRUE(grid.canPlace(exactlyGap));
}

TEST(SpacingGridTest, RejectsVerticallyOverlappingBoxesWithoutVerticalGap) {
    SpacingGrid grid(CELL_SIZE, HALF_GAP);

    SpacingBox lower = {0.f, 0.f, 100, 25};
    SpacingBox upper = {0.f, 50.f, 100, 25}; // overlaps in X, vertical gap = 25 < 100

    ASSERT_TRUE(grid.canPlace(lower));
    grid.insert(lower);

    EXPECT_FALSE(grid.canPlace(upper));
}

TEST(SpacingGridTest, CheckCountGrowsLinearly) {
    const int smallCount = 100;
    const int largeCount = 200;

    SpacingGrid small(CELL_SIZE, HALF_GAP);
    for (int i = 0; i < smallCount; ++i) {
        SpacingBox box = {i * 200.f, 0.f, 100, 25};
        ASSERT_TRUE(small.canPlace(box));
        small.insert(box);
    }

    SpacingGrid large(CELL_SIZE, HALF_GAP);
    for (int i = 0; i < largeCount; ++i) {
        SpacingBox box = {i * 200.f, 0.f, 100, 25};
        ASSERT_TRUE(large.canPlace(box));
        large.insert(box);
    }

    int checksSmall = small.getCheckCount();
    int checksLarge = large.getCheckCount();

    // Linear growth doubles the checks; quadratic growth would roughly quadruple
    // them. Allow some slack for cell bookkeeping.
    EXPECT_GT(checksSmall, 0);
    EXPECT_LE(checksLarge, 3 * checksSmall);
}
