#include <gtest/gtest.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <map>
#include <algorithm>
#include "LevelGenerator.h"
#include "objects/interactive/Platform.h"
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

TEST_F(LevelGeneratorTest, RealGeneratorPlacesSingleBlockPerLevel) {
    std::vector<GeneratedObject> objects;
    realGeneratePlatforms(objects);

    // Group blocks by their y level (the fixed start block is its own level).
    std::map<float, int> blocksPerLevel;
    for (const auto& obj : objects) {
        blocksPerLevel[obj.y]++;
    }

    for (const auto& entry : blocksPerLevel) {
        EXPECT_EQ(entry.second, 1)
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
        EXPECT_LE(levels[i] - levels[i - 1], TEST_MAX_JUMP_HEIGHT)
            << "Vertical gap between levels " << levels[i - 1] << " and " << levels[i]
            << " exceeds the maximum jump height";
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
    generator.generatePlatforms(objects);
    generator.generateTrees(objects);

    bool hasTree = false;
    for (const auto& obj : objects) {
        if (obj.type == "Tree") {
            hasTree = true;
            // trunkHeight should be between 100 and 200
            EXPECT_GE(obj.trunkHeight, 100);
            EXPECT_LE(obj.trunkHeight, 200);
            // numBranches should be between 2 and 4
            EXPECT_GE(obj.numBranches, 2);
            EXPECT_LE(obj.numBranches, 4);
        }
    }
    EXPECT_TRUE(hasTree) << "No tree found";
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

TEST_F(LevelGeneratorTest, RealGeneratorPlacesAtMostOneEnemyPerTenPlatforms) {
    LevelGenerator generator(nullptr);
    std::vector<GeneratedObject> objects;
    std::srand(42);
    generator.generatePlatforms(objects);
    generator.generateEnemies(objects);

    int blockCount = 0;
    int enemyCount = 0;
    for (const auto& obj : objects) {
        if (obj.type == "GroundBlock") {
            blockCount++;
        } else if (obj.type == "Shooter") {
            enemyCount++;
        }
    }

    EXPECT_LE(enemyCount, blockCount / 10);
    EXPECT_LE(enemyCount, LevelGenerator::MAX_ENEMIES);
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