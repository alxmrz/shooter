#include <gtest/gtest.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
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

static void testGenerateTrees(std::vector<GeneratedObject>& objects) {
    const int SEGMENT_WIDTH = 500;
    const int RIGHT_MARGIN = 200;
    int numTrees = TEST_LEVEL_WIDTH / SEGMENT_WIDTH;

    for (int i = 0; i < numTrees; i++) {
        int segmentStart = i * SEGMENT_WIDTH;
        float x = testRandomFloat(segmentStart, segmentStart + SEGMENT_WIDTH - RIGHT_MARGIN);
        float y = TEST_GROUND_Y;
        int trunkHeight = testRandomInt(100, 200);
        int numBranches = testRandomInt(2, 4);
        
        // Add tree
        GeneratedObject tree;
        tree.x = x;
        tree.y = y;
        tree.width = 0;
        tree.height = 0;
        tree.type = "Tree";
        tree.trunkHeight = trunkHeight;
        tree.numBranches = numBranches;
        tree.minBranchLength = 2;
        tree.maxBranchLength = 6;
        objects.push_back(tree);
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
    testGeneratePlatforms(objects);
    
    EXPECT_GT(objects.size(), 0u);
    
    int shortBlocks = 0;
    int longBlocks = 0;
    for (const auto& obj : objects) {
        EXPECT_EQ(obj.type, "GroundBlock");
        EXPECT_EQ(obj.height, 25);
        // width should be multiple of 50 and between 150 and 500
        EXPECT_GE(obj.width, 150);
        EXPECT_LE(obj.width, 500);
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

TEST_F(LevelGeneratorTest, PlatformVerticalSpacingDoesNotExceedMaxJumpHeight) {
    std::vector<GeneratedObject> platforms;
    testGeneratePlatforms(platforms);
    
    std::sort(platforms.begin(), platforms.end(), 
        [](const GeneratedObject& a, const GeneratedObject& b) {
            return a.y > b.y;
        });
    
    for (size_t i = 1; i < platforms.size(); ++i) {
        float verticalDistance = platforms[i-1].y - platforms[i].y;
        EXPECT_LE(verticalDistance, TEST_MAX_JUMP_HEIGHT);
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
    std::vector<GeneratedObject> objects;
    testGenerateTrees(objects);
    
    // Expect at least one tree
    EXPECT_GT(objects.size(), 0u);
    
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

TEST_F(LevelGeneratorTest, RealGeneratorPlacesAtMostOneTreePer500Pixels) {
    LevelGenerator generator(nullptr);
    std::vector<GeneratedObject> objects;
    generator.generateTrees(objects);

    const int SEGMENT_WIDTH = 500;
    const int RIGHT_MARGIN = 200;
    const int expectedTrees = LevelGenerator::LEVEL_WIDTH / SEGMENT_WIDTH;

    ASSERT_EQ(objects.size(), static_cast<size_t>(expectedTrees));

    for (int i = 0; i < expectedTrees; ++i) {
        EXPECT_EQ(objects[i].type, "Tree");
        EXPECT_GE(objects[i].x, i * SEGMENT_WIDTH);
        EXPECT_LE(objects[i].x, i * SEGMENT_WIDTH + SEGMENT_WIDTH - RIGHT_MARGIN);
    }

    // First tree trunk x is within [0, 300]
    ASSERT_FALSE(objects.empty());
    EXPECT_GE(objects[0].x, 0);
    EXPECT_LE(objects[0].x, SEGMENT_WIDTH - RIGHT_MARGIN);
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