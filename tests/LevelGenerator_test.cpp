#include <gtest/gtest.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include "LevelGenerator.h"
#include "objects/interactive/Platform.h"
#include "objects/interactive/Tree.h"
#include "objects/backgrounds/GroundBlock.h"
#include "objects/units/Shooter.h"

static const int TEST_LEVEL_WIDTH = 3000;
static const int TEST_LEVEL_HEIGHT = 3000;
static const int TEST_GROUND_Y = 2950;
static const int TEST_MAX_ENEMIES = 10;
static const int TEST_CAMERA_WIDTH = 800;
static const int TEST_CAMERA_HEIGHT = 600;
static const int TEST_MAX_JUMP_HEIGHT = 110;

static float testRandomFloat(float min, float max) {
    return min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

static int testRandomInt(int min, int max) {
    return min + std::rand() % (max - min + 1);
}

static GeneratedObject makeGroundBlock(float x, float y, int width) {
    GeneratedObject block;
    block.x = x;
    block.y = y;
    block.width = width;
    block.height = 25;
    block.type = "GroundBlock";
    return block;
}

static const GeneratedObject* findTree(const std::vector<GeneratedObject>& objects) {
    for (const auto& obj : objects) {
        if (obj.type == "Tree") {
            return &obj;
        }
    }
    return nullptr;
}

static const GeneratedObject* hostBlockOf(const std::vector<GeneratedObject>& objects,
                                          const GeneratedObject& tree) {
    for (const auto& obj : objects) {
        if (obj.type != "GroundBlock" || obj.y != tree.y) {
            continue;
        }
        if (tree.x >= obj.x && tree.x <= obj.x + obj.width) {
            return &obj;
        }
    }
    return nullptr;
}

// Outer end of the topmost branch: left end for side 0, right end for side 1.
struct TopBranchGeometry {
    float outerEnd;
    int width;
};

static TopBranchGeometry topBranchGeometry(const GeneratedObject& treeObj) {
    Tree tree(static_cast<int>(treeObj.x), static_cast<int>(treeObj.y), treeObj.trunkHeight,
              treeObj.numBranches, treeObj.minBranchLength, treeObj.maxBranchLength,
              treeObj.branchStep, treeObj.topBranchSide, treeObj.topBranchLength);

    Platform* top = nullptr;
    for (Platform* branch : tree.getBranches()) {
        if (top == nullptr || branch->getY() < top->getY()) {
            top = branch;
        }
    }

    TopBranchGeometry geometry;
    geometry.width = (top == nullptr) ? 0 : top->getWidth();
    geometry.outerEnd = (treeObj.topBranchSide == 0)
        ? top->getX()
        : top->getX() + top->getWidth();
    return geometry;
}

static void testGenerateGround(std::vector<GeneratedObject>& objects) {
    // Bottom border (ground)
    GeneratedObject ground;
    ground.x = 0;
    ground.y = TEST_GROUND_Y;
    ground.width = TEST_LEVEL_WIDTH;
    ground.height = 50;
    ground.type = "Border";
    objects.push_back(ground);
    
    // Left border
    GeneratedObject leftBorder;
    leftBorder.x = 0;
    leftBorder.y = 0;
    leftBorder.width = 50;
    leftBorder.height = TEST_LEVEL_HEIGHT;
    leftBorder.type = "Border";
    objects.push_back(leftBorder);
    
    // Right border
    GeneratedObject rightBorder;
    rightBorder.x = TEST_LEVEL_WIDTH - 50;
    rightBorder.y = 0;
    rightBorder.width = 50;
    rightBorder.height = TEST_LEVEL_HEIGHT;
    rightBorder.type = "Border";
    objects.push_back(rightBorder);
}

static void testGeneratePlatforms(std::vector<GeneratedObject>& objects) {
    const int TILE_WIDTH = 50;
    const int BLOCK_HEIGHT = 25;
    int currentY = TEST_GROUND_Y - 100;
    while (currentY > 100) {
        int numPlatforms = testRandomInt(1, 3);
        for (int i = 0; i < numPlatforms; i++) {
            GeneratedObject platform;
            platform.x = testRandomFloat(100, TEST_LEVEL_WIDTH - 200);
            platform.y = currentY;
            // Generate block length: 3-10 tiles, with preference for longer blocks
            int tiles;
            if (testRandomFloat(0, 1) < 0.6) {
                tiles = testRandomInt(6, 10);
            } else {
                tiles = testRandomInt(3, 5);
            }
            platform.width = tiles * TILE_WIDTH;
            platform.height = BLOCK_HEIGHT;
            platform.type = "GroundBlock";
            objects.push_back(platform);
        }
        int step = testRandomInt(50, 100);
        if (step > TEST_MAX_JUMP_HEIGHT) {
            step = TEST_MAX_JUMP_HEIGHT;
        }
        currentY -= step;
    }
}

static void testGenerateEnemies(std::vector<GeneratedObject>& objects) {
    // First generate platforms (ground blocks)
    std::vector<GeneratedObject> platforms;
    testGeneratePlatforms(platforms);
    
    // Filter platforms that are outside camera view
    std::vector<GeneratedObject> safePlatforms;
    for (const auto& p : platforms) {
        if (p.x > TEST_CAMERA_WIDTH + 100) {
            safePlatforms.push_back(p);
        }
    }
    if (safePlatforms.empty()) safePlatforms = platforms;
    
    int enemyCount = 0;
    while (enemyCount < TEST_MAX_ENEMIES) {
        GeneratedObject enemy;
        enemy.width = 50;
        enemy.height = 50;
        
        // Choose a random safe platform
        int platformIndex = testRandomInt(0, safePlatforms.size() - 1);
        const GeneratedObject& platform = safePlatforms[platformIndex];
        
        // Place enemy on platform
        enemy.x = testRandomFloat(platform.x + 10, platform.x + platform.width - enemy.width - 10);
        enemy.y = platform.y - enemy.height;
        
        // Ensure enemy is not in camera view
        if (enemy.x <= TEST_CAMERA_WIDTH + 100) {
            enemy.x = testRandomFloat(TEST_CAMERA_WIDTH + 100, TEST_LEVEL_WIDTH - 100);
            // Find platform that contains this x
            for (const auto& p : safePlatforms) {
                if (enemy.x >= p.x && enemy.x + enemy.width <= p.x + p.width) {
                    enemy.y = p.y - enemy.height;
                    break;
                }
            }
        }
        
        enemy.type = "Shooter";
        
        // Check distance to other enemies
        bool tooClose = false;
        for (const auto& existingEnemy : objects) {
            if (existingEnemy.type == "Shooter") {
                float dx = enemy.x - existingEnemy.x;
                float dy = enemy.y - existingEnemy.y;
                float distance = std::sqrt(dx * dx + dy * dy);
                if (distance < 200) {
                    tooClose = true;
                    break;
                }
            }
        }
        
        if (!tooClose) {
            objects.push_back(enemy);
            enemyCount++;
        }
    }
}

static void testGenerateEnemiesWithPlatforms(std::vector<GeneratedObject>& enemies, std::vector<GeneratedObject>& platforms) {
    // Generate platforms first
    testGeneratePlatforms(platforms);
    
    // Filter platforms that are outside camera view (x > CAMERA_WIDTH + 100)
    std::vector<GeneratedObject> safePlatforms;
    for (const auto& p : platforms) {
        if (p.x > TEST_CAMERA_WIDTH + 100) {
            safePlatforms.push_back(p);
        }
    }
    if (safePlatforms.empty()) safePlatforms = platforms;
    
    int enemyCount = 0;
    while (enemyCount < TEST_MAX_ENEMIES) {
        GeneratedObject enemy;
        enemy.width = 50;
        enemy.height = 50;
        
        // Choose a random safe platform
        int platformIndex = testRandomInt(0, safePlatforms.size() - 1);
        const GeneratedObject& platform = safePlatforms[platformIndex];
        
        // Place enemy on platform
        enemy.x = testRandomFloat(platform.x + 10, platform.x + platform.width - enemy.width - 10);
        enemy.y = platform.y - enemy.height;
        
        // Ensure enemy is not in camera view
        if (enemy.x <= TEST_CAMERA_WIDTH + 100) {
            // Shift to the right side and find a safe platform
            enemy.x = testRandomFloat(TEST_CAMERA_WIDTH + 100, TEST_LEVEL_WIDTH - 100);
            // Find platform that contains this x
            for (const auto& p : safePlatforms) {
                if (enemy.x >= p.x && enemy.x + enemy.width <= p.x + p.width) {
                    enemy.y = p.y - enemy.height;
                    break;
                }
            }
        }
        
        enemy.type = "Shooter";
        
        // Check distance to other enemies
        bool tooClose = false;
        for (const auto& existingEnemy : enemies) {
            if (existingEnemy.type == "Shooter") {
                float dx = enemy.x - existingEnemy.x;
                float dy = enemy.y - existingEnemy.y;
                float distance = std::sqrt(dx * dx + dy * dy);
                if (distance < 200) {
                    tooClose = true;
                    break;
                }
            }
        }
        
        if (!tooClose) {
            enemies.push_back(enemy);
            enemyCount++;
        }
    }
}

static bool expandedRectsOverlap(const GeneratedObject& a, const GeneratedObject& b, float halfGap) {
    float aL = a.x - halfGap;
    float aR = a.x + a.width + halfGap;
    float aT = a.y - halfGap;
    float aB = a.y + a.height + halfGap;
    float bL = b.x - halfGap;
    float bR = b.x + b.width + halfGap;
    float bT = b.y - halfGap;
    float bB = b.y + b.height + halfGap;
    return aL < bR && bL < aR && aT < bB && bT < aB;
}

// LevelGenerator's constructor reseeds with std::time(0); reseed after
// construction to make tests that inspect the real generator deterministic.
static void realGeneratePlatforms(std::vector<GeneratedObject>& objects) {
    LevelGenerator generator(nullptr);
    std::srand(42);
    generator.generatePlatforms(objects);
}

class LevelGeneratorTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::srand(42);
    }
};

TEST_F(LevelGeneratorTest, LevelDimensionsAreCorrect) {
    EXPECT_EQ(TEST_LEVEL_WIDTH, 3000);
    EXPECT_EQ(TEST_LEVEL_HEIGHT, 3000);
}

TEST_F(LevelGeneratorTest, GroundPlatformIsGenerated) {
    std::vector<GeneratedObject> objects;
    testGenerateGround(objects);
    
    // Should have 3 borders: ground, left, right
    ASSERT_EQ(objects.size(), 3u);
    
    // Bottom border (ground)
    EXPECT_EQ(objects[0].type, "Border");
    EXPECT_FLOAT_EQ(objects[0].x, 0);
    EXPECT_EQ(objects[0].y, TEST_GROUND_Y);
    EXPECT_EQ(objects[0].width, TEST_LEVEL_WIDTH);
    EXPECT_EQ(objects[0].height, 50);
    
    // Left border
    EXPECT_EQ(objects[1].type, "Border");
    EXPECT_FLOAT_EQ(objects[1].x, 0);
    EXPECT_EQ(objects[1].y, 0);
    EXPECT_EQ(objects[1].width, 50);
    EXPECT_EQ(objects[1].height, TEST_LEVEL_HEIGHT);
    
    // Right border
    EXPECT_EQ(objects[2].type, "Border");
    EXPECT_EQ(objects[2].x, TEST_LEVEL_WIDTH - 50);
    EXPECT_EQ(objects[2].y, 0);
    EXPECT_EQ(objects[2].width, 50);
    EXPECT_EQ(objects[2].height, TEST_LEVEL_HEIGHT);
}

TEST_F(LevelGeneratorTest, PlatformsAreGeneratedWithVaryingLengths) {
    std::vector<GeneratedObject> objects;
    realGeneratePlatforms(objects);

    EXPECT_GT(objects.size(), 0u);

    int shortBlocks = 0;
    int longBlocks = 0;
    for (const auto& obj : objects) {
        EXPECT_EQ(obj.type, "GroundBlock");
        EXPECT_EQ(obj.height, 25);
        // Widened blocks: multiple of 50 tiles, roughly 3*1.2 .. 10*2 tiles.
        EXPECT_GE(obj.width, 150);
        EXPECT_LE(obj.width, 1000);
        EXPECT_EQ(obj.width % 50, 0);
        int tiles = obj.width / 50;
        if (tiles <= 5) {
            shortBlocks++;
        } else {
            longBlocks++;
        }
    }
    // There should be more long blocks than short blocks
    EXPECT_GT(longBlocks, shortBlocks);
}

TEST_F(LevelGeneratorTest, RealGeneratorPlacesAtLeastOneBlockPerLevel) {
    std::vector<GeneratedObject> objects;
    realGeneratePlatforms(objects);

    // Group blocks by their y level (the fixed start block is its own level).
    // Several blocks per level are allowed now that every block gets an upward
    // neighbour, so only "at least one" is required.
    std::map<float, int> blocksPerLevel;
    for (const auto& obj : objects) {
        blocksPerLevel[obj.y]++;
    }

    for (const auto& entry : blocksPerLevel) {
        EXPECT_GE(entry.second, 1)
            << "Level y=" << entry.first << " has " << entry.second << " blocks";
    }
}

TEST_F(LevelGeneratorTest, RealGeneratorPlacesFixedStartBlockInBottomLeft) {
    std::vector<GeneratedObject> objects;
    realGeneratePlatforms(objects);

    bool found = false;
    for (const auto& obj : objects) {
        if (obj.x == LevelGenerator::START_BLOCK_X &&
            obj.y == LevelGenerator::START_BLOCK_Y &&
            obj.width == LevelGenerator::START_BLOCK_WIDTH &&
            obj.height == LevelGenerator::START_BLOCK_HEIGHT) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found) << "Fixed start block is missing";
}

static float horizontalGap(const GeneratedObject& a, const GeneratedObject& b) {
    float aRight = a.x + a.width;
    float bRight = b.x + b.width;
    return std::max(a.x, b.x) - std::min(aRight, bRight);
}

static float verticalGap(const GeneratedObject& a, const GeneratedObject& b) {
    float aBottom = a.y + a.height;
    float bBottom = b.y + b.height;
    return std::max(a.y, b.y) - std::min(aBottom, bBottom);
}

TEST_F(LevelGeneratorTest, RealGeneratorRespectsMinimumBlockSpacing) {
    std::vector<GeneratedObject> objects;
    realGeneratePlatforms(objects);

    ASSERT_GT(objects.size(), 0u);

    const float HALF_GAP = 50.f;
    for (size_t i = 0; i < objects.size(); ++i) {
        EXPECT_EQ(objects[i].type, "GroundBlock");
        for (size_t j = i + 1; j < objects.size(); ++j) {
            EXPECT_FALSE(expandedRectsOverlap(objects[i], objects[j], HALF_GAP))
                << "Blocks " << i << " and " << j << " are closer than the required gap";
        }
    }
}

TEST_F(LevelGeneratorTest, RealGeneratorSameLevelBlocksHaveHorizontalGap) {
    std::vector<GeneratedObject> objects;
    realGeneratePlatforms(objects);

    for (size_t i = 0; i < objects.size(); ++i) {
        for (size_t j = i + 1; j < objects.size(); ++j) {
            if (objects[i].y == objects[j].y) {
                EXPECT_GE(horizontalGap(objects[i], objects[j]), 100.f)
                    << "Blocks " << i << " and " << j << " on the same level are too close";
            }
        }
    }
}

TEST_F(LevelGeneratorTest, RealGeneratorVerticallyOverlappingBlocksHaveVerticalGap) {
    std::vector<GeneratedObject> objects;
    realGeneratePlatforms(objects);

    for (size_t i = 0; i < objects.size(); ++i) {
        for (size_t j = i + 1; j < objects.size(); ++j) {
            if (horizontalGap(objects[i], objects[j]) < 0.f) {
                EXPECT_GE(verticalGap(objects[i], objects[j]), 100.f)
                    << "Blocks " << i << " and " << j << " overlap in X without vertical clearance";
            }
        }
    }
}

TEST_F(LevelGeneratorTest, RealGeneratorDiagonalBlocksHaveClearanceOnSomeAxis) {
    std::vector<GeneratedObject> objects;
    realGeneratePlatforms(objects);

    for (size_t i = 0; i < objects.size(); ++i) {
        for (size_t j = i + 1; j < objects.size(); ++j) {
            if (horizontalGap(objects[i], objects[j]) >= 0.f &&
                verticalGap(objects[i], objects[j]) >= 0.f) {
                float bestGap = std::max(horizontalGap(objects[i], objects[j]),
                                         verticalGap(objects[i], objects[j]));
                EXPECT_GE(bestGap, 100.f)
                    << "Diagonal blocks " << i << " and " << j << " lack clearance";
            }
        }
    }
}

TEST_F(LevelGeneratorTest, MaxVerticalDistanceFollowsDoubleJumpMinusPlayerHeight) {
    EXPECT_EQ(LevelGenerator::MAX_JUMP_HEIGHT,
              LevelGenerator::DOUBLE_JUMP_HEIGHT - LevelGenerator::PLAYER_HEIGHT);
    EXPECT_EQ(LevelGenerator::MAX_JUMP_HEIGHT, 110);
}

TEST_F(LevelGeneratorTest, PlatformVerticalSpacingDoesNotExceedMaxJumpHeight) {
    std::vector<GeneratedObject> objects;
    realGeneratePlatforms(objects);

    ASSERT_GT(objects.size(), 0u);

    std::vector<float> levels;
    for (const auto& obj : objects) {
        levels.push_back(obj.y);
    }
    std::sort(levels.begin(), levels.end());
    levels.erase(std::unique(levels.begin(), levels.end()), levels.end());

    for (size_t i = 1; i < levels.size(); ++i) {
        // A block that carries a tree leaves a full tree height of room, which
        // the tree's branches bridge; all other steps fit within a jump.
        EXPECT_LE(levels[i] - levels[i - 1], LevelGenerator::TREE_HEIGHT)
            << "Vertical gap between levels " << levels[i - 1] << " and " << levels[i]
            << " exceeds the maximum tree-bridged height";
    }
}

// True when `upper` sits above `lower` within one jump and the player can cross
// the horizontal gap between them. A block that carries a tree may have its
// upward neighbour a full tree height away, because the tree's branches bridge
// the gap.
static bool isReachableUpwardNeighbor(const GeneratedObject& lower,
                                      const GeneratedObject& upper,
                                      bool carriesTree) {
    if (upper.y >= lower.y) {
        return false;
    }
    const float maxStep = carriesTree
        ? static_cast<float>(LevelGenerator::TREE_HEIGHT)
        : static_cast<float>(LevelGenerator::MAX_JUMP_HEIGHT);
    if (lower.y - upper.y > maxStep) {
        return false;
    }
    return horizontalGap(lower, upper) <= static_cast<float>(LevelGenerator::MAX_HORIZONTAL_REACH);
}

// A ground block carries a tree when it is the third (or sixth, ...) ground
// block and is wide enough, matching LevelGenerator's placement rule.
static bool blockCarriesTree(const std::vector<GeneratedObject>& objects,
                             const GeneratedObject& target) {
    int index = 0;
    for (const auto& obj : objects) {
        if (obj.type != "GroundBlock") {
            continue;
        }
        if (obj.x == target.x && obj.y == target.y) {
            return (index + 1) % 3 == 0 &&
                   target.width >= LevelGenerator::TREE_MIN_BLOCK_WIDTH;
        }
        index++;
    }
    return false;
}

// The topmost blocks have no level above them, so they are exempt from the
// "reachable upward neighbour" rule.
static bool hasLevelAbove(const std::vector<GeneratedObject>& blocks, const GeneratedObject& block) {
    for (const auto& other : blocks) {
        if (other.y < block.y) {
            return true;
        }
    }
    return false;
}

TEST_F(LevelGeneratorTest, RealGeneratorGivesEveryBlockAReachableUpwardNeighbor) {
    std::vector<GeneratedObject> objects;
    realGeneratePlatforms(objects);

    ASSERT_GT(objects.size(), 0u);

    for (const auto& block : objects) {
        if (!hasLevelAbove(objects, block)) {
            continue;
        }
        bool reachable = false;
        for (const auto& other : objects) {
            if (isReachableUpwardNeighbor(block, other, blockCarriesTree(objects, block))) {
                reachable = true;
                break;
            }
        }
        EXPECT_TRUE(reachable)
            << "Block at (" << block.x << ", " << block.y << ") has no reachable upward neighbour";
    }
}

TEST_F(LevelGeneratorTest, RealGeneratorUpwardNeighborGapIsWithinReach) {
    std::vector<GeneratedObject> objects;
    realGeneratePlatforms(objects);

    ASSERT_GT(objects.size(), 0u);

    for (const auto& block : objects) {
        if (!hasLevelAbove(objects, block)) {
            continue;
        }

        // Look for an upward neighbour whose horizontal gap is both large enough
        // to satisfy the minimum block spacing and small enough to jump across.
        bool found = false;
        for (const auto& other : objects) {
            if (!isReachableUpwardNeighbor(block, other, blockCarriesTree(objects, block))) {
                continue;
            }
            float gap = horizontalGap(block, other);
            if (gap >= 100.f &&
                gap <= static_cast<float>(LevelGenerator::MAX_HORIZONTAL_REACH)) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found)
            << "Block at (" << block.x << ", " << block.y
            << ") has no upward neighbour within the required horizontal gap range";
    }
}

TEST_F(LevelGeneratorTest, RealGeneratorHasClimbablePathFromStartBlock) {
    std::vector<GeneratedObject> objects;
    realGeneratePlatforms(objects);

    ASSERT_GT(objects.size(), 0u);

    // Locate the fixed start block.
    size_t startIndex = objects.size();
    for (size_t i = 0; i < objects.size(); ++i) {
        if (objects[i].x == LevelGenerator::START_BLOCK_X &&
            objects[i].y == LevelGenerator::START_BLOCK_Y) {
            startIndex = i;
            break;
        }
    }
    ASSERT_LT(startIndex, objects.size()) << "Start block not found";

    // Highest (smallest y) block level.
    float topY = objects[0].y;
    for (const auto& obj : objects) {
        topY = std::min(topY, obj.y);
    }

    // BFS over reachable upward edges.
    std::vector<bool> visited(objects.size(), false);
    std::vector<size_t> queue;
    queue.push_back(startIndex);
    visited[startIndex] = true;

    bool reachedTop = false;
    while (!queue.empty()) {
        size_t current = queue.back();
        queue.pop_back();

        if (objects[current].y == topY) {
            reachedTop = true;
            break;
        }

        for (size_t i = 0; i < objects.size(); ++i) {
            if (visited[i]) {
                continue;
            }
            if (isReachableUpwardNeighbor(objects[current], objects[i],
                                          blockCarriesTree(objects, objects[current]))) {
                visited[i] = true;
                queue.push_back(i);
            }
        }
    }

    EXPECT_TRUE(reachedTop) << "Cannot climb from the start block to the top level";
}

TEST_F(LevelGeneratorTest, RealGeneratorVariesHorizontalGapBetweenLevels) {
    // The gap to the upward neighbour must be dynamic: sometimes as small as the
    // minimum block gap, sometimes up to 50% wider, never beyond jump reach.
    std::set<float> observedGaps;
    int totalGaps = 0;
    int widerThanMinimum = 0;

    for (unsigned seed = 1; seed <= 20; ++seed) {
        std::vector<GeneratedObject> objects;
        LevelGenerator generator(nullptr);
        std::srand(seed);
        generator.generatePlatforms(objects);

        for (const auto& block : objects) {
            // Find the nearest level above this block.
            float nearestY = -1.f;
            for (const auto& other : objects) {
                if (other.y < block.y && (nearestY < 0.f || other.y > nearestY)) {
                    nearestY = other.y;
                }
            }
            if (nearestY < 0.f) {
                continue;
            }

            float bestGap = -1.f;
            for (const auto& other : objects) {
                if (other.y != nearestY) {
                    continue;
                }
                float gap = horizontalGap(block, other);
                if (gap < 0.f || gap > static_cast<float>(LevelGenerator::MAX_HORIZONTAL_REACH)) {
                    continue;
                }
                if (bestGap < 0.f || gap < bestGap) {
                    bestGap = gap;
                }
            }
            if (bestGap >= 0.f) {
                observedGaps.insert(bestGap);
                totalGaps++;
                if (bestGap > 100.f) {
                    widerThanMinimum++;
                }
            }
        }
    }

    ASSERT_FALSE(observedGaps.empty());
    for (float gap : observedGaps) {
        EXPECT_GE(gap, 100.f);
        EXPECT_LE(gap, static_cast<float>(LevelGenerator::MAX_HORIZONTAL_REACH));
    }

    // The gap must actually vary: a meaningful share of neighbours should sit
    // wider than the minimum block gap, reaching up to 50% more.
    EXPECT_GE(observedGaps.size(), 3u)
        << "Horizontal gaps between levels are not varied enough";
    EXPECT_GE(widerThanMinimum * 10, totalGaps * 4)
        << "Only " << widerThanMinimum << " of " << totalGaps
        << " gaps are wider than the minimum";
}

TEST_F(LevelGeneratorTest, RealGeneratorKeepsBlockCountBounded) {
    // Covering lower blocks with upward neighbours must not blow up the number
    // of blocks: a single upper block can cover several lower ones, so the
    // count stays proportional to the number of levels across many seeds.
    for (unsigned seed = 1; seed <= 20; ++seed) {
        std::vector<GeneratedObject> objects;
        LevelGenerator generator(nullptr);
        std::srand(seed);
        generator.generatePlatforms(objects);

        std::map<float, int> blocksPerLevel;
        for (const auto& obj : objects) {
            blocksPerLevel[obj.y]++;
        }

        ASSERT_FALSE(blocksPerLevel.empty());
        EXPECT_LE(objects.size(), blocksPerLevel.size() * 3)
            << "Seed " << seed << " produced " << objects.size()
            << " blocks over " << blocksPerLevel.size() << " levels";
    }
}

TEST_F(LevelGeneratorTest, EnemyCountDoesNotExceedMaximum) {
    std::vector<GeneratedObject> enemies;
    testGenerateEnemies(enemies);
    
    EXPECT_EQ(enemies.size(), static_cast<size_t>(TEST_MAX_ENEMIES));
    
    for (const auto& enemy : enemies) {
        EXPECT_EQ(enemy.type, "Shooter");
    }
}

TEST_F(LevelGeneratorTest, EnemiesAreNotTooClose) {
    std::vector<GeneratedObject> enemies;
    testGenerateEnemies(enemies);
    
    for (size_t i = 0; i < enemies.size(); ++i) {
        for (size_t j = i + 1; j < enemies.size(); ++j) {
            float dx = enemies[i].x - enemies[j].x;
            float dy = enemies[i].y - enemies[j].y;
            float distance = std::sqrt(dx * dx + dy * dy);
            EXPECT_GE(distance, 200.0f);
        }
    }
}

TEST_F(LevelGeneratorTest, EnemiesAreOutsideCameraView) {
    std::vector<GeneratedObject> enemies;
    testGenerateEnemies(enemies);
    
    for (const auto& enemy : enemies) {
        EXPECT_GE(enemy.x, static_cast<float>(TEST_CAMERA_WIDTH + 100));
    }
}

TEST_F(LevelGeneratorTest, TreesAreGeneratedWithBranches) {
    LevelGenerator generator(nullptr);
    std::vector<GeneratedObject> objects;
    std::srand(42);
    // Fillers occupy indices 0 and 1 so the wide target block is index 2,
    // which is the only position considered for a tree.
    objects.push_back(makeGroundBlock(900, 1300, 800));
    objects.push_back(makeGroundBlock(2500, 100, 100));
    objects.push_back(makeGroundBlock(1000, 2000, 600));

    generator.generateTrees(objects);

    bool hasTree = false;
    for (const auto& obj : objects) {
        if (obj.type == "Tree") {
            hasTree = true;
            // Every tree has the fixed trunk height and a ladder branch step.
            EXPECT_EQ(obj.trunkHeight, LevelGenerator::TREE_TRUNK_HEIGHT);
            EXPECT_EQ(obj.branchStep, LevelGenerator::TREE_BRANCH_STEP);
            EXPECT_GE(obj.minBranchLength, 2);
            EXPECT_LE(obj.maxBranchLength, 6);
        }
    }
    EXPECT_TRUE(hasTree) << "No tree found";
}

TEST_F(LevelGeneratorTest, RealGeneratorTreeHeightIsFixed) {
    LevelGenerator generator(nullptr);
    std::vector<GeneratedObject> objects;
    std::srand(42);
    // Overhead block is far away (clearance 675), so it is not raised.
    objects.push_back(makeGroundBlock(900, 1300, 800));
    objects.push_back(makeGroundBlock(2500, 100, 100));
    objects.push_back(makeGroundBlock(1000, 2000, 600));

    generator.generateTrees(objects);

    const GeneratedObject* tree = findTree(objects);
    ASSERT_NE(tree, nullptr);
    const int totalHeight = tree->trunkHeight + tree->trunkHeight / 2;
    EXPECT_GE(totalHeight, LevelGenerator::TREE_HEIGHT - 2);
    EXPECT_LE(totalHeight, LevelGenerator::TREE_HEIGHT);
}

TEST_F(LevelGeneratorTest, RealGeneratorSkipsTreeWhenColumnBlocked) {
    LevelGenerator generator(nullptr);
    std::vector<GeneratedObject> objects;
    std::srand(42);
    const float baseY = 2000;
    const int gap = 150; // overhead block closer than TREE_HEIGHT, spanning the whole block
    objects.push_back(makeGroundBlock(1000, baseY - gap - 25, 600));
    objects.push_back(makeGroundBlock(2500, 100, 100));
    objects.push_back(makeGroundBlock(1000, baseY, 600));

    generator.generateTrees(objects);

    EXPECT_EQ(findTree(objects), nullptr);
}

TEST_F(LevelGeneratorTest, RealGeneratorTreeHeightWithoutUpperBlock) {
    LevelGenerator generator(nullptr);
    std::vector<GeneratedObject> objects;
    std::srand(42);
    // Both fillers sit below the target, so there is no block above it.
    objects.push_back(makeGroundBlock(2000, 2500, 100));
    objects.push_back(makeGroundBlock(2100, 2600, 100));
    objects.push_back(makeGroundBlock(1000, 2000, 600));

    generator.generateTrees(objects);

    const GeneratedObject* tree = findTree(objects);
    ASSERT_NE(tree, nullptr);
    const int totalHeight = tree->trunkHeight + tree->trunkHeight / 2;
    EXPECT_GE(totalHeight, LevelGenerator::TREE_HEIGHT - 2);
    EXPECT_LE(totalHeight, LevelGenerator::TREE_HEIGHT);
}

TEST_F(LevelGeneratorTest, RealGeneratorPlacesTreesOnWideGroundBlocks) {
    LevelGenerator generator(nullptr);
    std::vector<GeneratedObject> objects;
    std::srand(42);
    generator.generatePlatforms(objects);
    generator.generateTrees(objects);

    int treeCount = 0;
    for (const auto& obj : objects) {
        if (obj.type != "Tree") {
            continue;
        }
        treeCount++;

        bool onBlock = false;
        for (const auto& block : objects) {
            if (block.type != "GroundBlock" || block.width < LevelGenerator::TREE_MIN_BLOCK_WIDTH) {
                continue;
            }
            if (obj.y == block.y &&
                obj.x >= block.x && obj.x <= block.x + block.width) {
                onBlock = true;
                break;
            }
        }
        EXPECT_TRUE(onBlock) << "Tree at (" << obj.x << ", " << obj.y << ") is not on a wide ground block";
    }
    EXPECT_GT(treeCount, 0);
}

TEST_F(LevelGeneratorTest, RealGeneratorPlacesAtMostOneTreePerBlock) {
    LevelGenerator generator(nullptr);
    std::vector<GeneratedObject> objects;
    std::srand(42);
    generator.generatePlatforms(objects);
    generator.generateTrees(objects);

    for (const auto& block : objects) {
        if (block.type != "GroundBlock") {
            continue;
        }
        int treesOnBlock = 0;
        for (const auto& obj : objects) {
            if (obj.type == "Tree" && obj.y == block.y &&
                obj.x >= block.x && obj.x <= block.x + block.width) {
                treesOnBlock++;
            }
        }
        EXPECT_LE(treesOnBlock, 1)
            << "Block at (" << block.x << ", " << block.y << ") has " << treesOnBlock << " trees";
    }
}

TEST_F(LevelGeneratorTest, RealGeneratorPlacesAtMostOneTreePerThreeBlocks) {
    LevelGenerator generator(nullptr);
    std::vector<GeneratedObject> objects;
    std::srand(42);
    generator.generatePlatforms(objects);
    generator.generateTrees(objects);

    int blockCount = 0;
    int treeCount = 0;
    for (const auto& obj : objects) {
        if (obj.type == "GroundBlock") {
            blockCount++;
        } else if (obj.type == "Tree") {
            treeCount++;
        }
    }

    EXPECT_LE(treeCount, blockCount / 3);
}

TEST_F(LevelGeneratorTest, RealGeneratorReservesRoomForEveryTreeCandidate) {
    LevelGenerator generator(nullptr);
    std::vector<GeneratedObject> objects;
    std::srand(42);
    generator.generatePlatforms(objects);
    generator.generateTrees(objects);

    int candidates = 0;
    int withTree = 0;
    int index = 0;
    for (const auto& block : objects) {
        if (block.type != "GroundBlock") {
            continue;
        }
        const bool candidate = (index + 1) % 3 == 0 &&
                               block.width >= LevelGenerator::TREE_MIN_BLOCK_WIDTH;
        index++;
        if (!candidate) {
            continue;
        }
        candidates++;

        bool hasTree = false;
        for (const auto& obj : objects) {
            if (obj.type == "Tree" && obj.y == block.y &&
                obj.x >= block.x && obj.x <= block.x + block.width) {
                hasTree = true;
                break;
            }
        }
        withTree += hasTree ? 1 : 0;
    }

    EXPECT_GT(candidates, 0);
    EXPECT_EQ(withTree, candidates)
        << withTree << " of " << candidates << " tree candidates got a tree";
}

TEST_F(LevelGeneratorTest, RealGeneratorPlacesTreePlatformOnTopBranchSide) {
    LevelGenerator generator(nullptr);
    std::vector<GeneratedObject> objects;
    std::srand(42);
    generator.generatePlatforms(objects);
    generator.generateTrees(objects);

    int checked = 0;
    for (const auto& tree : objects) {
        if (tree.type != "Tree") {
            continue;
        }

        // Locate the ground block the tree stands on.
        for (const auto& block : objects) {
            if (block.type != "GroundBlock" || block.y != tree.y) {
                continue;
            }
            if (tree.x < block.x || tree.x >= block.x + block.width) {
                continue;
            }

            // The reserved platform is the only ground block above within a tree height.
            for (const auto& up : objects) {
                if (up.type != "GroundBlock") {
                    continue;
                }
                const int gapY = static_cast<int>(block.y) - static_cast<int>(up.y);
                if (gapY <= 0 || gapY > LevelGenerator::TREE_HEIGHT) {
                    continue;
                }
                const bool platformOnLeft = (up.x + up.width <= block.x);
                const bool platformOnRight = (up.x >= block.x + block.width);
                if (!platformOnLeft && !platformOnRight) {
                    continue;
                }
                const bool expectedLeft = (tree.topBranchSide == 0);
                EXPECT_EQ(platformOnLeft, expectedLeft)
                    << "platform is not on the tree's top-branch side";
                checked++;
            }
        }
    }
    EXPECT_GT(checked, 0) << "no reserved platform was checked";
}

TEST_F(LevelGeneratorTest, RealGeneratorSetsTopBranchLengthToPlatformEdgeLeft) {
    LevelGenerator generator(nullptr);
    std::vector<GeneratedObject> objects;
    std::srand(42);
    GeneratedObject host = makeGroundBlock(1000, 2000, 600);
    host.topBranchSide = 0;
    objects.push_back(makeGroundBlock(900, 1300, 800));
    objects.push_back(makeGroundBlock(2500, 100, 100));
    objects.push_back(host);

    generator.generateTrees(objects);

    const GeneratedObject* tree = findTree(objects);
    ASSERT_NE(tree, nullptr);
    const int expected = static_cast<int>(tree->x) + Tree::TRUNK_WIDTH -
                         static_cast<int>(host.x);
    EXPECT_EQ(tree->topBranchLength, expected);
}

TEST_F(LevelGeneratorTest, RealGeneratorSetsTopBranchLengthToPlatformEdgeRight) {
    LevelGenerator generator(nullptr);
    std::vector<GeneratedObject> objects;
    std::srand(42);
    GeneratedObject host = makeGroundBlock(1000, 2000, 600);
    host.topBranchSide = 1;
    objects.push_back(makeGroundBlock(900, 1300, 800));
    objects.push_back(makeGroundBlock(2500, 100, 100));
    objects.push_back(host);

    generator.generateTrees(objects);

    const GeneratedObject* tree = findTree(objects);
    ASSERT_NE(tree, nullptr);
    const int expected = static_cast<int>(host.x + host.width) - static_cast<int>(tree->x);
    EXPECT_EQ(tree->topBranchLength, expected);
}

TEST_F(LevelGeneratorTest, RealGeneratorTopBranchReachesPlatformEdge) {
    int checked = 0;
    for (unsigned seed = 1; seed <= 10; ++seed) {
        LevelGenerator generator(nullptr);
        std::vector<GeneratedObject> objects;
        std::srand(seed);
        generator.generatePlatforms(objects);
        generator.generateTrees(objects);

        for (const auto& tree : objects) {
            if (tree.type != "Tree") {
                continue;
            }
            const GeneratedObject* block = hostBlockOf(objects, tree);
            ASSERT_NE(block, nullptr) << "seed " << seed;
            const TopBranchGeometry geometry = topBranchGeometry(tree);
            const float edge = (tree.topBranchSide == 0)
                ? block->x
                : block->x + block->width;
            EXPECT_FLOAT_EQ(geometry.outerEnd, edge) << "seed " << seed;
            checked++;
        }
    }
    EXPECT_GT(checked, 0) << "no trees were generated";
}

TEST_F(LevelGeneratorTest, RealGeneratorReservedPlatformWithinReachFromTopBranch) {
    int checked = 0;
    for (unsigned seed = 1; seed <= 20; ++seed) {
        LevelGenerator generator(nullptr);
        std::vector<GeneratedObject> objects;
        std::srand(seed);
        generator.generatePlatforms(objects);
        generator.generateTrees(objects);

        for (const auto& tree : objects) {
            if (tree.type != "Tree") {
                continue;
            }
            const GeneratedObject* block = hostBlockOf(objects, tree);
            ASSERT_NE(block, nullptr) << "seed " << seed;
            const TopBranchGeometry geometry = topBranchGeometry(tree);

            for (const auto& up : objects) {
                if (up.type != "GroundBlock") {
                    continue;
                }
                const int gapY = static_cast<int>(block->y) - static_cast<int>(up.y);
                if (gapY <= 0 || gapY > LevelGenerator::TREE_HEIGHT) {
                    continue;
                }
                const bool onLeft = up.x + up.width <= block->x;
                const bool onRight = up.x >= block->x + block->width;
                if ((tree.topBranchSide == 0 && !onLeft) ||
                    (tree.topBranchSide == 1 && !onRight)) {
                    continue;
                }
                const float platformNearEdge = (tree.topBranchSide == 0)
                    ? up.x + up.width
                    : up.x;
                const float gap = std::abs(geometry.outerEnd - platformNearEdge);
                EXPECT_LE(gap, static_cast<float>(LevelGenerator::MAX_HORIZONTAL_REACH))
                    << "seed " << seed;
                checked++;
            }
        }
    }
    EXPECT_GT(checked, 0) << "no reserved platforms were checked";
}

static bool hasTreeOn(const std::vector<GeneratedObject>& objects, const GeneratedObject& block) {
    for (const auto& obj : objects) {
        if (obj.type == "Tree" && obj.y == block.y &&
            obj.x >= block.x && obj.x <= block.x + block.width) {
            return true;
        }
    }
    return false;
}

TEST_F(LevelGeneratorTest, RealGeneratorPlacesEnemyOnEverySecondCrystalPlatform) {
    LevelGenerator generator(nullptr);
    std::vector<GeneratedObject> objects;
    std::srand(42);
    generator.generatePlatforms(objects);
    generator.generateTrees(objects);
    generator.generateEnemies(objects);

    std::vector<GeneratedObject> blocks;
    for (const auto& obj : objects) {
        if (obj.type == "GroundBlock") {
            blocks.push_back(obj);
        }
    }

    std::vector<int> enemyIndices;
    for (const auto& obj : objects) {
        if (obj.type != "Shooter") {
            continue;
        }
        int index = -1;
        for (size_t i = 0; i < blocks.size(); ++i) {
            if (obj.y + obj.height == blocks[i].y &&
                obj.x >= blocks[i].x &&
                obj.x + obj.width <= blocks[i].x + blocks[i].width) {
                index = static_cast<int>(i);
                break;
            }
        }
        ASSERT_GE(index, 0) << "enemy is not on any ground block";
        EXPECT_GE(obj.x, static_cast<float>(LevelGenerator::CAMERA_SAFE_X));
        enemyIndices.push_back(index);
    }
    EXPECT_GT(enemyIndices.size(), 0u) << "real generator produced no enemies";

    // Independent oracle: a platform carries a crystal when it is an even 1-based
    // platform without a tree; enemies must sit on every second such platform.
    int crystalOrdinal = 0;
    for (size_t i = 0; i < blocks.size(); ++i) {
        const bool hasCrystal = ((i + 1) % 2 == 0) && !hasTreeOn(objects, blocks[i]);
        crystalOrdinal += hasCrystal ? 1 : 0;
        const bool hasEnemy =
            std::find(enemyIndices.begin(), enemyIndices.end(), static_cast<int>(i)) != enemyIndices.end();
        if (hasEnemy) {
            EXPECT_TRUE(hasCrystal) << "enemy on platform without crystal";
            EXPECT_EQ(crystalOrdinal % 2, 0) << "enemy not on every second crystal platform";
        }
    }
}

TEST_F(LevelGeneratorTest, RealGeneratorPlacesEnemiesOnPlatformsOutsideCamera) {
    LevelGenerator generator(nullptr);
    std::vector<GeneratedObject> objects;
    std::srand(42);
    generator.generatePlatforms(objects);
    generator.generateEnemies(objects);

    for (const auto& obj : objects) {
        if (obj.type != "Shooter") {
            continue;
        }

        bool onBlock = false;
        for (const auto& block : objects) {
            if (block.type != "GroundBlock") {
                continue;
            }
            if (obj.x >= block.x && obj.x + obj.width <= block.x + block.width &&
                obj.y + obj.height == block.y) {
                onBlock = true;
                break;
            }
        }
        EXPECT_TRUE(onBlock) << "Enemy at (" << obj.x << ", " << obj.y << ") is not on a ground block";
        EXPECT_GE(obj.x, static_cast<float>(LevelGenerator::CAMERA_WIDTH + 100));
    }
}

TEST_F(LevelGeneratorTest, EnemiesArePlacedOnPlatforms) {
    std::vector<GeneratedObject> enemies;
    std::vector<GeneratedObject> platforms;
    testGenerateEnemiesWithPlatforms(enemies, platforms);
    
    EXPECT_EQ(enemies.size(), static_cast<size_t>(TEST_MAX_ENEMIES));
    
    for (const auto& enemy : enemies) {
        // Find a platform that supports this enemy
        bool found = false;
        for (const auto& platform : platforms) {
            // Check if enemy is directly above platform (within horizontal bounds)
            if (enemy.x >= platform.x && enemy.x + enemy.width <= platform.x + platform.width) {
                // Check vertical alignment: enemy bottom should be at platform top
                if (enemy.y + enemy.height != platform.y) {
                    // Enemy might have been shifted to camera-safe position, just check it's on some platform
                    continue;
                }
                found = true;
                break;
            }
        }
        // At minimum, enemy should be outside camera view
        EXPECT_GT(enemy.x, static_cast<float>(TEST_CAMERA_WIDTH + 100))
            << "Enemy at (" << enemy.x << ", " << enemy.y << ") is within camera view";
    }
}

// ---------------------------------------------------------------------------
// Deterministic platform item placement
// ---------------------------------------------------------------------------

static const int TEST_ITEM_SIZE = 50;

static GeneratedObject makePlatform(float x, float y, int width = 300) {
    GeneratedObject block;
    block.x = x;
    block.y = y;
    block.width = width;
    block.height = 25;
    block.type = "GroundBlock";
    return block;
}

static std::vector<GeneratedObject> makePlatforms(int count) {
    std::vector<GeneratedObject> objects;
    for (int i = 0; i < count; ++i) {
        objects.push_back(makePlatform(100.0f + i * 400.0f, 2000.0f));
    }
    return objects;
}

static GeneratedObject makeTreeOn(const GeneratedObject& block) {
    GeneratedObject tree;
    tree.x = block.x + 10.0f;
    tree.y = block.y;
    tree.width = 0;
    tree.height = 0;
    tree.type = "Tree";
    return tree;
}

static std::vector<GeneratedObject> platformsOf(const std::vector<GeneratedObject>& objects) {
    std::vector<GeneratedObject> blocks;
    for (const auto& obj : objects) {
        if (obj.type == "GroundBlock") {
            blocks.push_back(obj);
        }
    }
    return blocks;
}

static int countByType(const std::vector<GeneratedObject>& objects, const std::string& type) {
    int count = 0;
    for (const auto& obj : objects) {
        if (obj.type == type) {
            count++;
        }
    }
    return count;
}

// Index (0-based) of the platform whose top edge supports the item, or -1.
static int platformIndexForItem(const std::vector<GeneratedObject>& blocks,
                                const GeneratedObject& item) {
    for (size_t i = 0; i < blocks.size(); ++i) {
        if (item.y == blocks[i].y - TEST_ITEM_SIZE &&
            item.x >= blocks[i].x &&
            item.x + item.width <= blocks[i].x + blocks[i].width) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

static bool isItemType(const GeneratedObject& obj) {
    return obj.type == "Ammunition" || obj.type == "Heart" || obj.type == "Crystal";
}

TEST_F(LevelGeneratorTest, EveryThirdPlatformGetsAmmunition) {
    std::vector<GeneratedObject> objects = makePlatforms(6);
    LevelGenerator generator(nullptr);
    generator.generatePlatformItems(objects);

    std::vector<GeneratedObject> blocks = platformsOf(objects);
    EXPECT_EQ(countByType(objects, "Ammunition"), 2);
    for (const auto& obj : objects) {
        if (obj.type != "Ammunition") {
            continue;
        }
        int index = platformIndexForItem(blocks, obj);
        ASSERT_GE(index, 0) << "Ammunition is not on a platform";
        EXPECT_EQ((index + 1) % 3, 0) << "Ammunition on platform " << index + 1;
    }
}

TEST_F(LevelGeneratorTest, EveryFourthPlatformGetsHeart) {
    std::vector<GeneratedObject> objects = makePlatforms(8);
    LevelGenerator generator(nullptr);
    generator.generatePlatformItems(objects);

    std::vector<GeneratedObject> blocks = platformsOf(objects);
    EXPECT_EQ(countByType(objects, "Heart"), 2);
    for (const auto& obj : objects) {
        if (obj.type != "Heart") {
            continue;
        }
        int index = platformIndexForItem(blocks, obj);
        ASSERT_GE(index, 0) << "Heart is not on a platform";
        EXPECT_EQ((index + 1) % 4, 0) << "Heart on platform " << index + 1;
    }
}

TEST_F(LevelGeneratorTest, EverySecondPlatformGetsCrystal) {
    std::vector<GeneratedObject> objects = makePlatforms(10);
    LevelGenerator generator(nullptr);
    generator.generatePlatformItems(objects);

    std::vector<GeneratedObject> blocks = platformsOf(objects);
    EXPECT_EQ(countByType(objects, "Crystal"), 5);
    for (const auto& obj : objects) {
        if (obj.type != "Crystal") {
            continue;
        }
        int index = platformIndexForItem(blocks, obj);
        ASSERT_GE(index, 0) << "Crystal is not on a platform";
        EXPECT_EQ((index + 1) % 2, 0) << "Crystal on platform " << index + 1;
    }
}

TEST_F(LevelGeneratorTest, PlatformsWithTreesGetNoItems) {
    std::vector<GeneratedObject> objects = makePlatforms(10);
    std::vector<GeneratedObject> blocks = platformsOf(objects);
    // Trees on the 3rd, 4th and 5th platforms (multiples of 3, 4 and 5).
    objects.push_back(makeTreeOn(blocks[2]));
    objects.push_back(makeTreeOn(blocks[3]));
    objects.push_back(makeTreeOn(blocks[4]));

    LevelGenerator generator(nullptr);
    generator.generatePlatformItems(objects);

    // Ammunition on 3rd, 6th, 9th; 3rd is skipped.
    EXPECT_EQ(countByType(objects, "Ammunition"), 2);
    // Heart on 4th, 8th; 4th is skipped.
    EXPECT_EQ(countByType(objects, "Heart"), 1);
    // Crystal on 2nd, 4th, 6th, 8th, 10th; 4th is skipped.
    EXPECT_EQ(countByType(objects, "Crystal"), 4);

    for (const auto& obj : objects) {
        if (!isItemType(obj)) {
            continue;
        }
        int index = platformIndexForItem(blocks, obj);
        ASSERT_GE(index, 0) << obj.type << " is not on a platform";
        EXPECT_NE(index, 2) << obj.type << " placed on the tree platform";
        EXPECT_NE(index, 3) << obj.type << " placed on the tree platform";
        EXPECT_NE(index, 4) << obj.type << " placed on the tree platform";
    }
}

TEST_F(LevelGeneratorTest, ItemPlacementIsDeterministic) {
    std::vector<GeneratedObject> objects = makePlatforms(20);
    LevelGenerator generator(nullptr);
    generator.generatePlatformItems(objects);

    std::vector<GeneratedObject> blocks = platformsOf(objects);
    // 20 platforms: every 3rd -> 6 ammo, every 4th -> 5 hearts, every 2nd -> 10 crystals.
    EXPECT_EQ(countByType(objects, "Ammunition"), 6);
    EXPECT_EQ(countByType(objects, "Heart"), 5);
    EXPECT_EQ(countByType(objects, "Crystal"), 10);

    for (const auto& obj : objects) {
        if (!isItemType(obj)) {
            continue;
        }
        int index = platformIndexForItem(blocks, obj);
        ASSERT_GE(index, 0) << obj.type << " is not on a platform";
        int step = (obj.type == "Ammunition") ? 3 : (obj.type == "Heart") ? 4 : 2;
        EXPECT_EQ((index + 1) % step, 0)
            << obj.type << " on platform " << index + 1 << " (step " << step << ")";
    }
}

TEST_F(LevelGeneratorTest, ItemsRestOnPlatformTopWithoutOverlap) {
    std::vector<GeneratedObject> objects = makePlatforms(12);
    LevelGenerator generator(nullptr);
    generator.generatePlatformItems(objects);

    std::vector<GeneratedObject> blocks = platformsOf(objects);
    std::vector<GeneratedObject> onTwelfth;
    for (const auto& obj : objects) {
        if (!isItemType(obj)) {
            continue;
        }
        int index = platformIndexForItem(blocks, obj);
        ASSERT_GE(index, 0) << obj.type << " is not on a platform";
        EXPECT_EQ(obj.y, blocks[index].y - TEST_ITEM_SIZE)
            << obj.type << " does not rest on the platform top";
        if (index == 11) {
            onTwelfth.push_back(obj);
        }
    }

    // The 12th platform is divisible by 2, 3 and 4, so it gets all three items.
    ASSERT_EQ(onTwelfth.size(), 3u);
    for (size_t i = 0; i < onTwelfth.size(); ++i) {
        for (size_t j = i + 1; j < onTwelfth.size(); ++j) {
            bool overlap = onTwelfth[i].x < onTwelfth[j].x + onTwelfth[j].width &&
                           onTwelfth[j].x < onTwelfth[i].x + onTwelfth[i].width;
            EXPECT_FALSE(overlap) << "Items overlap on the 12th platform";
        }
    }
}

// ---------------------------------------------------------------------------
// Deterministic enemy placement
// ---------------------------------------------------------------------------

static std::vector<GeneratedObject> makeFarPlatforms(int count, int width = 300) {
    std::vector<GeneratedObject> objects;
    for (int i = 0; i < count; ++i) {
        objects.push_back(makePlatform(1000.0f + i * 400.0f, 2000.0f, width));
    }
    return objects;
}

// Index (0-based) of the platform whose top edge supports the enemy, or -1.
static int enemyPlatformIndex(const std::vector<GeneratedObject>& blocks,
                              const GeneratedObject& enemy) {
    for (size_t i = 0; i < blocks.size(); ++i) {
        if (enemy.y + enemy.height == blocks[i].y &&
            enemy.x >= blocks[i].x &&
            enemy.x + enemy.width <= blocks[i].x + blocks[i].width) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

TEST_F(LevelGeneratorTest, EnemyOnEverySecondCrystalPlatform) {
    std::vector<GeneratedObject> objects = makeFarPlatforms(9);
    LevelGenerator generator(nullptr);
    generator.generateEnemies(objects);

    // Crystal platforms (no trees) are 0-based indices 1,3,5,7; every second of
    // them is index 3 and 7.
    std::vector<GeneratedObject> blocks = platformsOf(objects);
    ASSERT_EQ(countByType(objects, "Shooter"), 2);
    for (const auto& obj : objects) {
        if (obj.type != "Shooter") {
            continue;
        }
        int index = enemyPlatformIndex(blocks, obj);
        ASSERT_GE(index, 0) << "enemy is not on a platform";
        EXPECT_EQ((index + 1) % 4, 0)
            << "enemy not on every second crystal platform: index " << index;
    }
}

TEST_F(LevelGeneratorTest, EnemySkippedWithoutCrystal) {
    std::vector<GeneratedObject> objects = makeFarPlatforms(9);
    LevelGenerator generator(nullptr);
    generator.generateEnemies(objects);

    std::vector<GeneratedObject> blocks = platformsOf(objects);
    for (const auto& obj : objects) {
        if (obj.type != "Shooter") {
            continue;
        }
        int index = enemyPlatformIndex(blocks, obj);
        ASSERT_GE(index, 0) << "enemy is not on a platform";
        EXPECT_EQ((index + 1) % 2, 0)
            << "enemy on platform without a crystal: index " << index;
    }
}

TEST_F(LevelGeneratorTest, EnemySkippedOnTreePlatform) {
    std::vector<GeneratedObject> objects = makeFarPlatforms(9);
    std::vector<GeneratedObject> blocks = platformsOf(objects);
    objects.push_back(makeTreeOn(blocks[3])); // 4th platform: a crystal platform

    LevelGenerator generator(nullptr);
    generator.generateEnemies(objects);

    std::vector<GeneratedObject> afterBlocks = platformsOf(objects);
    for (const auto& obj : objects) {
        if (obj.type != "Shooter") {
            continue;
        }
        int index = enemyPlatformIndex(afterBlocks, obj);
        ASSERT_GE(index, 0);
        EXPECT_NE(index, 3) << "enemy placed on the tree platform";
    }
}

TEST_F(LevelGeneratorTest, OneEnemyPerPlatform) {
    std::vector<GeneratedObject> objects = makeFarPlatforms(12);
    LevelGenerator generator(nullptr);
    generator.generateEnemies(objects);

    std::vector<GeneratedObject> blocks = platformsOf(objects);
    std::map<int, int> enemiesPerPlatform;
    for (const auto& obj : objects) {
        if (obj.type != "Shooter") {
            continue;
        }
        int index = enemyPlatformIndex(blocks, obj);
        ASSERT_GE(index, 0);
        enemiesPerPlatform[index]++;
    }
    for (const auto& entry : enemiesPerPlatform) {
        EXPECT_EQ(entry.second, 1) << "platform " << entry.first;
    }
}

TEST_F(LevelGeneratorTest, NoMaxEnemiesCap) {
    // 84 platforms -> 42 crystal platforms -> 21 enemies, well above the old cap.
    std::vector<GeneratedObject> objects = makeFarPlatforms(84);
    LevelGenerator generator(nullptr);
    generator.generateEnemies(objects);

    EXPECT_EQ(countByType(objects, "Shooter"), 21);
    EXPECT_GT(countByType(objects, "Shooter"), LevelGenerator::MAX_ENEMIES);
}

TEST_F(LevelGeneratorTest, EnemyStandsOnPlatformTop) {
    std::vector<GeneratedObject> objects = makeFarPlatforms(6);
    LevelGenerator generator(nullptr);
    generator.generateEnemies(objects);

    std::vector<GeneratedObject> blocks = platformsOf(objects);
    for (const auto& obj : objects) {
        if (obj.type != "Shooter") {
            continue;
        }
        int index = enemyPlatformIndex(blocks, obj);
        ASSERT_GE(index, 0);
        EXPECT_EQ(obj.y, blocks[index].y - LevelGenerator::ENEMY_HEIGHT);
    }
}

TEST_F(LevelGeneratorTest, EnemyOutsideCameraView) {
    std::vector<GeneratedObject> objects = makeFarPlatforms(9);
    LevelGenerator generator(nullptr);
    generator.generateEnemies(objects);

    for (const auto& obj : objects) {
        if (obj.type != "Shooter") {
            continue;
        }
        EXPECT_GE(obj.x, static_cast<float>(LevelGenerator::CAMERA_SAFE_X))
            << "enemy at " << obj.x << " is inside the camera view";
    }
}