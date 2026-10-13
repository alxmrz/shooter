#include <gtest/gtest.h>
#include <cstdlib>
#include <algorithm>
#include <set>
#include <vector>
#include "objects/interactive/Tree.h"
#include "objects/interactive/Platform.h"

class TreeTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::srand(42);
    }
};

TEST_F(TreeTest, BoundingBoxIsNonZeroAndCoversAllParts) {
    Tree tree(500, 950, 100, 3, 2, 6);

    EXPECT_GT(tree.getWidth(), 0);
    EXPECT_GT(tree.getHeight(), 0);

    auto covers = [&](Platform* part) {
        EXPECT_LE(tree.getX(), part->getX());
        EXPECT_LE(tree.getY(), part->getY());
        EXPECT_GE(tree.getX() + tree.getWidth(), part->getX() + part->getWidth());
        EXPECT_GE(tree.getY() + tree.getHeight(), part->getY() + part->getHeight());
    };

    covers(tree.getTrunk());
    covers(tree.getLeaves());
    for (Platform* branch : tree.getBranches()) {
        covers(branch);
    }
}

TEST_F(TreeTest, LeavesGeometry) {
    const int trunkX = 500;
    const int trunkHeight = 100;
    const int trunkWidth = Tree::TRUNK_WIDTH;

    Tree tree(trunkX, 950, trunkHeight, 3, 2, 6);
    Platform* leaves = tree.getLeaves();

    EXPECT_EQ(leaves->getWidth(), Tree::LEAVES_WIDTH_MULTIPLIER * trunkWidth);
    EXPECT_EQ(leaves->getHeight(), trunkHeight / 2);
    EXPECT_EQ(leaves->getX(), trunkX - Tree::LEAVES_X_OFFSET_MULTIPLIER * trunkWidth);
    EXPECT_FALSE(leaves->isCollidable());
}

TEST_F(TreeTest, TrunkGeometry) {
    const int trunkX = 500;
    const int baseY = 950;
    const int trunkHeight = 120;

    Tree tree(trunkX, baseY, trunkHeight, 3, 2, 6);
    Platform* trunk = tree.getTrunk();

    EXPECT_EQ(trunk->getWidth(), Tree::TRUNK_WIDTH);
    EXPECT_EQ(trunk->getHeight(), trunkHeight);
    EXPECT_FLOAT_EQ(trunk->getX(), trunkX);
    EXPECT_FLOAT_EQ(trunk->getY(), baseY - trunkHeight);
    EXPECT_FALSE(trunk->isCollidable());
}

TEST_F(TreeTest, BranchPlacementWithinTrunkExtendsOutward) {
    const int trunkX = 500;
    const int trunkHeight = 120;

    Tree tree(trunkX, 950, trunkHeight, 4, 2, 6);
    Platform* trunk = tree.getTrunk();
    const float trunkLeft = trunk->getX();
    const float trunkRight = trunk->getX() + trunk->getWidth();

    std::vector<Platform*> branches = tree.getBranches();
    ASSERT_EQ(branches.size(), 2u);

    for (Platform* branch : branches) {
        const float left = branch->getX();
        const float right = branch->getX() + branch->getWidth();

        const bool leftEdgeInside = left >= trunkLeft && left <= trunkRight;
        const bool rightEdgeInside = right >= trunkLeft && right <= trunkRight;
        EXPECT_TRUE(leftEdgeInside || rightEdgeInside) << "branch base is outside trunk x-range";

        EXPECT_TRUE(left < trunkLeft || right > trunkRight) << "branch does not extend outward";

        EXPECT_TRUE(branch->isCollidable());

        const int tiles = branch->getWidth() / Tree::TILE_WIDTH;
        EXPECT_GE(tiles, 2);
        EXPECT_LE(tiles, 6);
    }
}

TEST_F(TreeTest, BranchesAreUniqueAndSeparateFromTrunkAndLeaves) {
    Tree tree(500, 950, 120, 4, 2, 6);

    std::vector<Platform*> branches = tree.getBranches();
    std::set<Platform*> uniqueBranches(branches.begin(), branches.end());
    EXPECT_EQ(uniqueBranches.size(), branches.size());

    for (Platform* branch : branches) {
        EXPECT_NE(branch, tree.getTrunk());
        EXPECT_NE(branch, tree.getLeaves());
    }
}

TEST_F(TreeTest, AtMostOneBranchPerSide) {
    const int trunkX = 500;
    const int trunkHeight = 120;

    for (int requested = 2; requested <= 6; ++requested) {
        std::srand(100 + requested);
        Tree tree(trunkX, 950, trunkHeight, requested, 2, 6);

        Platform* trunk = tree.getTrunk();
        const float trunkLeft = trunk->getX();
        const float trunkRight = trunk->getX() + trunk->getWidth();

        int leftCount = 0;
        int rightCount = 0;
        for (Platform* branch : tree.getBranches()) {
            const float left = branch->getX();
            const float right = branch->getX() + branch->getWidth();
            if (left < trunkLeft) leftCount++;
            if (right > trunkRight) rightCount++;
        }

        EXPECT_LE(leftCount, 1) << "requested=" << requested;
        EXPECT_LE(rightCount, 1) << "requested=" << requested;
        EXPECT_EQ(leftCount, 1) << "requested=" << requested;
        EXPECT_EQ(rightCount, 1) << "requested=" << requested;
    }
}

TEST_F(TreeTest, LadderBranchesAreStepSpacedAndAlternateSides) {
    const int trunkX = 500;
    const int trunkHeight = 333;
    const int branchStep = 90;

    Tree tree(trunkX, 950, trunkHeight, 0, 2, 6, branchStep);

    std::vector<Platform*> branches = tree.getBranches();
    ASSERT_GT(branches.size(), 2u);

    std::vector<int> ys;
    for (Platform* branch : branches) {
        ys.push_back(branch->getY());
    }
    std::sort(ys.begin(), ys.end());
    for (size_t i = 1; i < ys.size(); ++i) {
        EXPECT_LE(ys[i] - ys[i - 1], branchStep);
    }

    Platform* trunk = tree.getTrunk();
    const float trunkLeft = trunk->getX();
    const float trunkRight = trunk->getX() + trunk->getWidth();
    int leftCount = 0;
    int rightCount = 0;
    for (Platform* branch : branches) {
        if (branch->getX() < trunkLeft) leftCount++;
        if (branch->getX() + branch->getWidth() > trunkRight) rightCount++;
    }
    EXPECT_GT(leftCount, 1);
    EXPECT_GT(rightCount, 1);
}

TEST_F(TreeTest, LadderBranchesStayOnTrunk) {
    const int trunkX = 500;
    const int trunkHeight = 266;
    const int branchStep = 90;

    Tree tree(trunkX, 950, trunkHeight, 0, 2, 6, branchStep);

    Platform* trunk = tree.getTrunk();
    const int trunkTop = static_cast<int>(trunk->getY());

    std::vector<Platform*> branches = tree.getBranches();
    ASSERT_GT(branches.size(), 0u);

    int highestBranchY = trunkTop;
    for (Platform* branch : branches) {
        EXPECT_GE(static_cast<int>(branch->getY()), trunkTop)
            << "branch extends above the trunk into the foliage";
        highestBranchY = std::min(highestBranchY, static_cast<int>(branch->getY()));
    }

    // The topmost branch must sit near the trunk top so the reserved platform
    // above the tree stays within a double jump.
    EXPECT_LE(highestBranchY - trunkTop, branchStep);
}

TEST_F(TreeTest, LadderTopBranchUsesRequestedLength) {
    const int trunkX = 500;
    const int trunkHeight = 266;
    const int branchStep = 90;
    const int topBranchLength = 300;

    Tree tree(trunkX, 950, trunkHeight, 0, 2, 6, branchStep, 0, topBranchLength);

    std::vector<Platform*> branches = tree.getBranches();
    ASSERT_GT(branches.size(), 0u);

    Platform* top = nullptr;
    for (Platform* branch : branches) {
        if (top == nullptr || branch->getY() < top->getY()) {
            top = branch;
        }
    }
    ASSERT_NE(top, nullptr);
    EXPECT_EQ(top->getWidth(), topBranchLength);

    for (Platform* branch : branches) {
        if (branch == top) {
            continue;
        }
        const int tiles = branch->getWidth() / Tree::TILE_WIDTH;
        EXPECT_GE(tiles, 2);
        EXPECT_LE(tiles, 6);
    }
}

TEST_F(TreeTest, LadderTopBranchBaseStaysInsideTrunk) {
    const int trunkX = 500;
    const int trunkHeight = 266;
    const int topBranchLength = 320;

    for (int side = 0; side <= 1; ++side) {
        Tree tree(trunkX, 950, trunkHeight, 0, 2, 6, 90, side, topBranchLength);

        Platform* trunk = tree.getTrunk();
        const float trunkLeft = trunk->getX();
        const float trunkRight = trunk->getX() + trunk->getWidth();

        Platform* top = nullptr;
        for (Platform* branch : tree.getBranches()) {
            if (top == nullptr || branch->getY() < top->getY()) {
                top = branch;
            }
        }
        ASSERT_NE(top, nullptr);

        const float left = top->getX();
        const float right = top->getX() + top->getWidth();
        const bool leftInside = left >= trunkLeft && left <= trunkRight;
        const bool rightInside = right >= trunkLeft && right <= trunkRight;
        EXPECT_TRUE(leftInside || rightInside) << "side=" << side;
        EXPECT_TRUE(left < trunkLeft || right > trunkRight) << "side=" << side;
        EXPECT_EQ(top->getWidth(), topBranchLength);
    }
}

TEST_F(TreeTest, LegacyTreeBranchesKeepRandomLength) {
    const int trunkX = 500;
    const int trunkHeight = 120;

    Tree tree(trunkX, 950, trunkHeight, 4, 2, 6);

    std::vector<Platform*> branches = tree.getBranches();
    ASSERT_GT(branches.size(), 0u);
    for (Platform* branch : branches) {
        const int tiles = branch->getWidth() / Tree::TILE_WIDTH;
        EXPECT_GE(tiles, 2);
        EXPECT_LE(tiles, 6);
    }
}

TEST_F(TreeTest, TopBranchIsOnRequestedSide) {
    const int trunkX = 500;
    const int trunkHeight = 266;

    for (int side = 0; side <= 1; ++side) {
        Tree tree(trunkX, 950, trunkHeight, 0, 2, 6, 90, side);
        Platform* trunk = tree.getTrunk();
        const float trunkLeft = trunk->getX();
        const float trunkRight = trunk->getX() + trunk->getWidth();

        Platform* top = nullptr;
        for (Platform* branch : tree.getBranches()) {
            if (!top || branch->getY() < top->getY()) {
                top = branch;
            }
        }
        ASSERT_NE(top, nullptr);

        if (side == 0) {
            EXPECT_LT(top->getX(), trunkLeft) << "top branch should extend left";
        } else {
            EXPECT_GT(top->getX() + top->getWidth(), trunkRight)
                << "top branch should extend right";
        }
    }
}
