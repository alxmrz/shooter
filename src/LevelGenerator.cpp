#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>
#include "LevelGenerator.h"
#include "Map.h"
#include "GameObjects.h"
#include "Fabric.h"
#include "objects/units/Shooter.h"
#include "objects/interactive/Platform.h"
#include "objects/interactive/Crystal.h"
#include "objects/interactive/Heart.h"
#include "objects/interactive/Ammunition.h"

constexpr int LevelGenerator::LEVEL_WIDTH;
constexpr int LevelGenerator::LEVEL_HEIGHT;
constexpr int LevelGenerator::GROUND_Y;
constexpr int LevelGenerator::MAX_ENEMIES;
constexpr int LevelGenerator::CAMERA_WIDTH;
constexpr int LevelGenerator::CAMERA_HEIGHT;
constexpr int LevelGenerator::MAX_JUMP_HEIGHT;

LevelGenerator::LevelGenerator(GameObjects* go) : go(go) {
    std::srand(std::time(0));
}

LevelGenerator::LevelGenerator(const LevelGenerator& orig) {
}

LevelGenerator::~LevelGenerator() {
}

void LevelGenerator::generateLevel(Map* map) {
    std::vector<GeneratedObject> objects;
    
    generateGround(objects);
    generatePlatforms(objects);
    generateItems(objects, objects);
    generateEnemies(objects);
    
    // Create player first at starting position
    Shooter* player = go->fabric->createShooter(100, GROUND_Y - 100, 50, 50);
    player->setMain(true);
    go->player = player;
    go->all.push_back(player);
    
    // Create objects using Fabric and add to GameObjects
    for (const auto& obj : objects) {
        if (obj.type == "Border") {
            go->borders.insert({static_cast<int>(obj.x), 
                go->fabric->createBorder(obj.x, obj.y, obj.width, obj.height)});
        } else if (obj.type == "Platform") {
            go->all.push_back(go->fabric->createPlatform(obj.x, obj.y, obj.width, obj.height));
        } else if (obj.type == "Crystal") {
            go->all.push_back(go->fabric->createCrystal(obj.x, obj.y, obj.width, obj.height));
        } else if (obj.type == "Heart") {
            go->all.push_back(go->fabric->createHeart(obj.x, obj.y, obj.width, obj.height));
        } else if (obj.type == "Ammunition") {
            go->all.push_back(go->fabric->createAmmo(obj.x, obj.y, obj.width, obj.height));
        } else if (obj.type == "Shooter") {
            Shooter* shooter = go->fabric->createShooter(obj.x, obj.y, obj.width, obj.height);
            go->all.push_back(shooter);
        }
    }
}

void LevelGenerator::generateGround(std::vector<GeneratedObject>& objects) {
    // Create ground platform along the bottom of the level
    GeneratedObject ground;
    ground.x = 0;
    ground.y = GROUND_Y;
    ground.width = LEVEL_WIDTH;
    ground.height = 50;
    ground.type = "Border";
    objects.push_back(ground);
    
    // Left border
    GeneratedObject leftBorder;
    leftBorder.x = 0;
    leftBorder.y = 0;
    leftBorder.width = 50;
    leftBorder.height = LEVEL_HEIGHT;
    leftBorder.type = "Border";
    objects.push_back(leftBorder);
    
    // Right border
    GeneratedObject rightBorder;
    rightBorder.x = LEVEL_WIDTH - 50;
    rightBorder.y = 0;
    rightBorder.width = 50;
    rightBorder.height = LEVEL_HEIGHT;
    rightBorder.type = "Border";
    objects.push_back(rightBorder);
}

void LevelGenerator::generatePlatforms(std::vector<GeneratedObject>& objects) {
    // Start from ground level and move upward
    int currentY = GROUND_Y - 100; // Start above ground
    
    while (currentY > 100) { // Stop before reaching top
        // Determine number of platforms at this level (1-3)
        int numPlatforms = randomInt(1, 3);
        
        for (int i = 0; i < numPlatforms; i++) {
            GeneratedObject platform;
            platform.x = randomFloat(100, LEVEL_WIDTH - 200);
            platform.y = currentY;
            platform.width = 100;
            platform.height = 25;
            platform.type = "Platform";
            objects.push_back(platform);
        }
        
        // Move up by random step (50-100 pixels)
        int step = randomInt(50, 100);
        currentY -= step;
        
        // Ensure we don't exceed maximum jump height
        if (step > MAX_JUMP_HEIGHT) {
            step = MAX_JUMP_HEIGHT;
        }
    }
}

void LevelGenerator::generateItems(std::vector<GeneratedObject>& objects, const std::vector<GeneratedObject>& platforms) {
    // Limit maximum items per type
    const int MAX_CRYSTALS = 20;
    const int MAX_HEARTS = 10;
    const int MAX_AMMO = 15;
    
    int crystalCount = 0;
    int heartCount = 0;
    int ammoCount = 0;
    
    // Place items on platforms
    for (const auto& platform : platforms) {
        if (platform.type != "Platform") continue;
        
        // 30% chance to place an item on each platform
        if (randomFloat(0, 1) < 0.3) {
            int itemType = randomInt(0, 2);
            
            GeneratedObject item;
            item.x = platform.x + randomFloat(10, platform.width - 60);
            item.y = platform.y - 50; // Place above platform
            item.width = 50;
            item.height = 50;
            
            switch (itemType) {
                case 0: // Crystal
                    if (crystalCount < MAX_CRYSTALS) {
                        item.type = "Crystal";
                        objects.push_back(item);
                        crystalCount++;
                    }
                    break;
                case 1: // Heart
                    if (heartCount < MAX_HEARTS) {
                        item.type = "Heart";
                        objects.push_back(item);
                        heartCount++;
                    }
                    break;
                case 2: // Ammunition
                    if (ammoCount < MAX_AMMO) {
                        item.type = "Ammunition";
                        objects.push_back(item);
                        ammoCount++;
                    }
                    break;
            }
        }
    }
    
    // Place items in air between platforms
    for (int y = 200; y < GROUND_Y - 200; y += 200) {
        if (randomFloat(0, 1) < 0.1) { // 10% chance
            GeneratedObject item;
            item.x = randomFloat(100, LEVEL_WIDTH - 200);
            item.y = y;
            item.width = 50;
            item.height = 50;
            
            int itemType = randomInt(0, 2);
            switch (itemType) {
                case 0:
                    if (crystalCount < MAX_CRYSTALS) {
                        item.type = "Crystal";
                        objects.push_back(item);
                        crystalCount++;
                    }
                    break;
                case 1:
                    if (heartCount < MAX_HEARTS) {
                        item.type = "Heart";
                        objects.push_back(item);
                        heartCount++;
                    }
                    break;
                case 2:
                    if (ammoCount < MAX_AMMO) {
                        item.type = "Ammunition";
                        objects.push_back(item);
                        ammoCount++;
                    }
                    break;
            }
        }
    }
}

void LevelGenerator::generateEnemies(std::vector<GeneratedObject>& objects) {
    int enemyCount = 0;
    
    // Place enemies at random positions, but not in camera view
    // Assume player starts near left side (x=0)
    // Place enemies in regions that are at least CAMERA_WIDTH away from starting position
    while (enemyCount < MAX_ENEMIES) {
        GeneratedObject enemy;
        
        // Place enemies in right half of level or far from left side
        float xRegion = randomFloat(0, 1);
        if (xRegion < 0.7) { // 70% chance to place in right half
            enemy.x = randomFloat(LEVEL_WIDTH / 2, LEVEL_WIDTH - 100);
        } else { // 30% chance to place in left half but far from start
            enemy.x = randomFloat(CAMERA_WIDTH + 100, LEVEL_WIDTH / 2);
        }
        
        enemy.y = randomFloat(100, GROUND_Y - 100);
        enemy.width = 50;
        enemy.height = 50;
        enemy.type = "Shooter";
        
        // Simple check to avoid placing too close to other enemies
        bool tooClose = false;
        for (const auto& existingEnemy : objects) {
            if (existingEnemy.type == "Shooter") {
                float dx = enemy.x - existingEnemy.x;
                float dy = enemy.y - existingEnemy.y;
                float distance = std::sqrt(dx * dx + dy * dy);
                if (distance < 200) { // Minimum distance between enemies
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

float LevelGenerator::randomFloat(float min, float max) {
    return min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

int LevelGenerator::randomInt(int min, int max) {
    return min + std::rand() % (max - min + 1);
}