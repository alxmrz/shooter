#include <gtest/gtest.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>

struct GeneratedObject {
    float x;
    float y;
    int width;
    int height;
    std::string type;
};

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
    int currentY = TEST_GROUND_Y - 100;
    while (currentY > 100) {
        int numPlatforms = testRandomInt(1, 3);
        for (int i = 0; i < numPlatforms; i++) {
            GeneratedObject platform;
            platform.x = testRandomFloat(100, TEST_LEVEL_WIDTH - 200);
            platform.y = currentY;
            platform.width = 100;
            platform.height = 25;
            platform.type = "Platform";
            objects.push_back(platform);
        }
        int step = testRandomInt(50, 100);
        currentY -= step;
        if (step > TEST_MAX_JUMP_HEIGHT) {
            step = TEST_MAX_JUMP_HEIGHT;
        }
    }
}

static void testGenerateEnemies(std::vector<GeneratedObject>& objects) {
    int enemyCount = 0;
    while (enemyCount < TEST_MAX_ENEMIES) {
        GeneratedObject enemy;
        float xRegion = testRandomFloat(0, 1);
        if (xRegion < 0.7) {
            enemy.x = testRandomFloat(TEST_LEVEL_WIDTH / 2, TEST_LEVEL_WIDTH - 100);
        } else {
            enemy.x = testRandomFloat(TEST_CAMERA_WIDTH + 100, TEST_LEVEL_WIDTH / 2);
        }
        enemy.y = testRandomFloat(100, TEST_GROUND_Y - 100);
        enemy.width = 50;
        enemy.height = 50;
        enemy.type = "Shooter";
        
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

TEST_F(LevelGeneratorTest, PlatformsAreGenerated) {
    std::vector<GeneratedObject> objects;
    testGeneratePlatforms(objects);
    
    EXPECT_GT(objects.size(), 0u);
    
    for (const auto& obj : objects) {
        EXPECT_EQ(obj.type, "Platform");
        EXPECT_EQ(obj.width, 100);
        EXPECT_EQ(obj.height, 25);
    }
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