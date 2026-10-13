#include <gtest/gtest.h>
#include <cstdlib>
#include <algorithm>
#include <set>
#include <vector>
#include "objects/interactive/Tree.h"
#include "objects/interactive/Platform.h"

namespace {

Platform* topBranchOf(Tree& tree) {
    Platform* top = nullptr;
    for (Platform* branch : tree.getBranches()) {
        if (top == nullptr || branch->getY() < top->getY()) {
            top = branch;
        }
    }
    return top;
}

void expectTreeCovers(Tree& tree, Platform* part) {
    EXPECT_LE(tree.getX(), part->getX());
    EXPECT_LE(tree.getY(), part->getY());
    EXPECT_GE(tree.getX() + tree.getWidth(), part->getX() + part->getWidth());
    EXPECT_GE(tree.getY() + tree.getHeight(), part->getY() + part->getHeight());
}

void expectTreeCoversAllParts(Tree& tree) {
    expectTreeCovers(tree, tree.getTrunk());
    expectTreeCovers(tree, tree.getLeaves());

    for (Platform* branch : tree.getBranches()) {
        expectTreeCovers(tree, branch);
    }
}

void expectInsideTrunkAndOutward(Platform* branch, float trunkLeft, float trunkRight) {
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

void expectAllBranchesInsideTrunkAndOutward(Tree& tree) {
    Platform* trunk = tree.getTrunk();
    const float trunkLeft = trunk->getX();
    const float trunkRight = trunk->getX() + trunk->getWidth();

    for (Platform* branch : tree.getBranches()) {
        expectInsideTrunkAndOutward(branch, trunkLeft, trunkRight);
    }
}

void expectBranchesUniqueAndSeparate(Tree& tree) {
    std::vector<Platform*> branches = tree.getBranches();
    std::set<Platform*> uniqueBranches(branches.begin(), branches.end());
    EXPECT_EQ(uniqueBranches.size(), branches.size());

    for (Platform* branch : branches) {
        EXPECT_NE(branch, tree.getTrunk());
        EXPECT_NE(branch, tree.getLeaves());
    }
}

void expectOneBranchPerSide(Tree& tree) {
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

    EXPECT_EQ(leftCount, 1);
    EXPECT_EQ(rightCount, 1);
}

void expectOneBranchPerSideForWidths(int from, int to) {
    for (int requested = from; requested <= to; ++requested) {
        std::srand(100 + requested);
        Tree tree(500, 950, 120, requested, 2, 6);
        expectOneBranchPerSide(tree);
    }
}

void expectBranchesStepSpaced(Tree& tree, int branchStep) {
    std::vector<int> ys;
    for (Platform* branch : tree.getBranches()) {
        ys.push_back(branch->getY());
    }
    std::sort(ys.begin(), ys.end());

    for (size_t i = 1; i < ys.size(); ++i) {
        EXPECT_LE(ys[i] - ys[i - 1], branchStep);
    }
}

void expectBranchesAlternateSides(Tree& tree) {
    Platform* trunk = tree.getTrunk();
    const float trunkLeft = trunk->getX();
    const float trunkRight = trunk->getX() + trunk->getWidth();

    int leftCount = 0;
    int rightCount = 0;
    for (Platform* branch : tree.getBranches()) {
        if (branch->getX() < trunkLeft) leftCount++;
        if (branch->getX() + branch->getWidth() > trunkRight) rightCount++;
    }

    EXPECT_GT(leftCount, 1);
    EXPECT_GT(rightCount, 1);
}

void expectBranchesNotAboveTrunk(Tree& tree, int branchStep) {
    Platform* trunk = tree.getTrunk();
    const int trunkTop = static_cast<int>(trunk->getY());
    int highestBranchY = trunkTop;

    for (Platform* branch : tree.getBranches()) {
        EXPECT_GE(static_cast<int>(branch->getY()), trunkTop)
            << "branch extends above the trunk into the foliage";
        highestBranchY = std::min(highestBranchY, static_cast<int>(branch->getY()));
    }

    EXPECT_LE(highestBranchY - trunkTop, branchStep);
}

void expectOtherBranchesRandomLength(Tree& tree, Platform* top) {
    for (Platform* branch : tree.getBranches()) {
        if (branch == top) {
            continue;
        }
        const int tiles = branch->getWidth() / Tree::TILE_WIDTH;
        EXPECT_GE(tiles, 2);
        EXPECT_LE(tiles, 6);
    }
}

void expectTopBranchBaseInsideTrunk(int side, int topBranchLength) {
    Tree tree(500, 950, 266, 0, 2, 6, 90, side, topBranchLength);

    Platform* trunk = tree.getTrunk();
    const float trunkLeft = trunk->getX();
    const float trunkRight = trunk->getX() + trunk->getWidth();

    Platform* top = topBranchOf(tree);
    ASSERT_NE(top, nullptr);

    const float left = top->getX();
    const float right = top->getX() + top->getWidth();
    const bool leftInside = left >= trunkLeft && left <= trunkRight;
    const bool rightInside = right >= trunkLeft && right <= trunkRight;
    EXPECT_TRUE(leftInside || rightInside) << "side=" << side;
    EXPECT_TRUE(left < trunkLeft || right > trunkRight) << "side=" << side;
    EXPECT_EQ(top->getWidth(), topBranchLength);
}

void expectTopBranchBaseInsideTrunkForSides(int topBranchLength) {
    for (int side = 0; side <= 1; ++side) {
        expectTopBranchBaseInsideTrunk(side, topBranchLength);
    }
}

void expectTopBranchOnSide(int side) {
    Tree tree(500, 950, 266, 0, 2, 6, 90, side);

    Platform* trunk = tree.getTrunk();
    const float trunkLeft = trunk->getX();
    const float trunkRight = trunk->getX() + trunk->getWidth();

    Platform* top = topBranchOf(tree);
    ASSERT_NE(top, nullptr);

    if (side == 0) {
        EXPECT_LT(top->getX(), trunkLeft) << "top branch should extend left";
    } else {
        EXPECT_GT(top->getX() + top->getWidth(), trunkRight)
            << "top branch should extend right";
    }
}

void expectTopBranchOnEverySide() {
    for (int side = 0; side <= 1; ++side) {
        expectTopBranchOnSide(side);
    }
}
}

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

    expectTreeCoversAllParts(tree);
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
    Tree tree(500, 950, 120, 4, 2, 6);

    EXPECT_EQ(tree.getBranches().size(), 2u);
    expectAllBranchesInsideTrunkAndOutward(tree);
}

TEST_F(TreeTest, BranchesAreUniqueAndSeparateFromTrunkAndLeaves) {
    Tree tree(500, 950, 120, 4, 2, 6);

    expectBranchesUniqueAndSeparate(tree);
}

TEST_F(TreeTest, AtMostOneBranchPerSide) {
    expectOneBranchPerSideForWidths(2, 6);
}

TEST_F(TreeTest, LadderBranchesAreStepSpacedAndAlternateSides) {
    const int branchStep = 90;
    Tree tree(500, 950, 333, 0, 2, 6, branchStep);

    EXPECT_GT(tree.getBranches().size(), 2u);
    expectBranchesStepSpaced(tree, branchStep);
    expectBranchesAlternateSides(tree);
}

TEST_F(TreeTest, LadderBranchesStayOnTrunk) {
    const int branchStep = 90;
    Tree tree(500, 950, 266, 0, 2, 6, branchStep);

    EXPECT_GT(tree.getBranches().size(), 0u);
    expectBranchesNotAboveTrunk(tree, branchStep);
}

TEST_F(TreeTest, LadderTopBranchUsesRequestedLength) {
    const int topBranchLength = 300;
    Tree tree(500, 950, 266, 0, 2, 6, 90, 0, topBranchLength);

    Platform* top = topBranchOf(tree);
    ASSERT_NE(top, nullptr);
    EXPECT_EQ(top->getWidth(), topBranchLength);

    expectOtherBranchesRandomLength(tree, top);
}

TEST_F(TreeTest, LadderTopBranchBaseStaysInsideTrunk) {
    expectTopBranchBaseInsideTrunkForSides(320);
}

TEST_F(TreeTest, LegacyTreeBranchesKeepRandomLength) {
    Tree tree(500, 950, 120, 4, 2, 6);

    EXPECT_GT(tree.getBranches().size(), 0u);
    expectOtherBranchesRandomLength(tree, nullptr);
}

TEST_F(TreeTest, TopBranchIsOnRequestedSide) {
    expectTopBranchOnEverySide();
}
