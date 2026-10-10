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
#include "objects/interactive/Heart.h"
#include "objects/interactive/Ammunition.h"
#include "objects/interactive/Tree.h"
#include "objects/backgrounds/GroundBlock.h"

constexpr int LevelGenerator::LEVEL_WIDTH;
constexpr int LevelGenerator::LEVEL_HEIGHT;
constexpr int LevelGenerator::GROUND_Y;
constexpr int LevelGenerator::MAX_ENEMIES;
constexpr int LevelGenerator::CAMERA_WIDTH;
constexpr int LevelGenerator::CAMERA_HEIGHT;
constexpr int LevelGenerator::MAX_JUMP_HEIGHT;

LevelGenerator::LevelGenerator(GameObjects* go) : go(go)
{
    std::srand(std::time(0));
}

LevelGenerator::LevelGenerator(const LevelGenerator& orig)
{
}

LevelGenerator::~LevelGenerator()
{
}

void LevelGenerator::generateLevel(Map* map)
{
    std::vector<GeneratedObject> objects;

    generateGround(objects);

    generatePlatforms(objects);

    generateTrees(objects);

    generateItems(objects, objects);

    generateEnemies(objects);


    // Create player first at starting position
    Shooter* player = go->fabric->createShooter(100, GROUND_Y - 110, 50, 50);
    player->setMain(true);
    go->player = player;
    go->all.push_back(player);

    // Create objects using Fabric and add to GameObjects
    for (const auto& obj : objects)
    {
        if (obj.type == "Border")
        {
            go->borders.insert({
                static_cast<int>(obj.x),
                go->fabric->createBorder(obj.x, obj.y, obj.width, obj.height)
            });
        }
        else if (obj.type == "Platform")
        {
            go->all.push_back(go->fabric->createPlatform(obj.x, obj.y, obj.width, obj.height));
        }
        else if (obj.type == "GroundBlock")
        {
            go->all.push_back(go->fabric->createGroundBlock(obj.x, obj.y, obj.width, obj.height));
        }
        else if (obj.type == "Tree")
        {
            Tree* tree = go->fabric->createTree(obj.x, obj.y, obj.trunkHeight, obj.numBranches, obj.minBranchLength,
                                                obj.maxBranchLength);
            // Add only branches to all (for collision)
            for (Platform* branch : tree->getBranches())
            {
                go->all.push_back(branch);
            }
            go->all.push_back(tree);
        }
        else if (obj.type == "Heart")
        {
            go->all.push_back(go->fabric->createHeart(obj.x, obj.y, obj.width, obj.height));
        }
        else if (obj.type == "Ammunition")
        {
            go->all.push_back(go->fabric->createAmmo(obj.x, obj.y, obj.width, obj.height));
        }
        else if (obj.type == "Shooter")
        {
            Shooter* shooter = go->fabric->createShooter(obj.x, obj.y, obj.width, obj.height);
            go->all.push_back(shooter);
        }
    }
}

void LevelGenerator::generateGround(std::vector<GeneratedObject>& objects)
{
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

void LevelGenerator::generatePlatforms(std::vector<GeneratedObject>& objects)
{
    // Generate ground blocks with varying lengths (3-10 tiles)
    // Tile width assumed to be 50 pixels (same as ground height)
    const int TILE_WIDTH = 50;
    const int BLOCK_HEIGHT = 25;

    // Start from ground level and move upward
    int currentY = GROUND_Y - 100; // Start above ground

    while (currentY > 100)
    {
        // Stop before reaching top
        // Determine number of blocks at this level (1-3)
        int numBlocks = randomInt(1, 3);

        for (int i = 0; i < numBlocks; i++)
        {
            GeneratedObject block;
            block.x = randomFloat(100, LEVEL_WIDTH - 200);
            block.y = currentY;

            // Generate block length: 3-10 tiles, with preference for longer blocks
            // 60% chance for long blocks (6-10 tiles), 40% for short (3-5 tiles)
            int tiles;
            if (randomFloat(0, 1) < 0.6)
            {
                tiles = randomInt(6, 10); // long block
            }
            else
            {
                tiles = randomInt(3, 5); // short block
            }
            block.width = tiles * TILE_WIDTH;
            block.height = BLOCK_HEIGHT;
            block.type = "GroundBlock";
            objects.push_back(block);
        }

        // Move up by random step (50-100 pixels), but ensure not more than MAX_JUMP_HEIGHT
        int step = randomInt(50, 100);
        if (step > MAX_JUMP_HEIGHT)
        {
            step = MAX_JUMP_HEIGHT;
        }
        currentY -= step;
    }
}

void LevelGenerator::generateItems(std::vector<GeneratedObject>& objects, const std::vector<GeneratedObject>& platforms)
{
    // Limit maximum items per type
    const int MAX_CRYSTALS = 20;
    const int MAX_HEARTS = 10;
    const int MAX_AMMO = 15;

    int crystalCount = 0;
    int heartCount = 0;
    int ammoCount = 0;

    // Place items on platforms
    for (const auto& platform : platforms)
    {
        if (platform.type != "Platform") continue;

        // 30% chance to place an item on each platform
        if (randomFloat(0, 1) < 0.3)
        {
            int itemType = randomInt(0, 2);

            GeneratedObject item;
            item.x = platform.x + randomFloat(10, platform.width - 60);
            item.y = platform.y - 50; // Place above platform
            item.width = 50;
            item.height = 50;

            switch (itemType)
            {
            case 0: // Crystal
                if (crystalCount < MAX_CRYSTALS)
                {
                    item.type = "Crystal";
                    objects.push_back(item);
                    crystalCount++;
                }
                break;
            case 1: // Heart
                if (heartCount < MAX_HEARTS)
                {
                    item.type = "Heart";
                    objects.push_back(item);
                    heartCount++;
                }
                break;
            case 2: // Ammunition
                if (ammoCount < MAX_AMMO)
                {
                    item.type = "Ammunition";
                    objects.push_back(item);
                    ammoCount++;
                }
                break;
            }
        }
    }

    // Place items in air between platforms
    for (int y = 200; y < GROUND_Y - 200; y += 200)
    {
        if (randomFloat(0, 1) < 0.1)
        {
            // 10% chance
            GeneratedObject item;
            item.x = randomFloat(100, LEVEL_WIDTH - 200);
            item.y = y;
            item.width = 50;
            item.height = 50;

            int itemType = randomInt(0, 2);
            switch (itemType)
            {
            case 0:
                if (crystalCount < MAX_CRYSTALS)
                {
                    item.type = "Crystal";
                    objects.push_back(item);
                    crystalCount++;
                }
                break;
            case 1:
                if (heartCount < MAX_HEARTS)
                {
                    item.type = "Heart";
                    objects.push_back(item);
                    heartCount++;
                }
                break;
            case 2:
                if (ammoCount < MAX_AMMO)
                {
                    item.type = "Ammunition";
                    objects.push_back(item);
                    ammoCount++;
                }
                break;
            }
        }
    }
}

void LevelGenerator::generateEnemies(std::vector<GeneratedObject>& objects)
{
    int enemyCount = 0;

    // Collect all platforms (type "Platform")
    std::vector<GeneratedObject> platforms;
    std::vector<GeneratedObject> shooters;
    for (const auto& obj : objects)
    {
        if (obj.type == "Platform")
        {
            platforms.push_back(obj);
        }
        else if (obj.type == "Shooter")
        {
            shooters.push_back(obj);
        }
    }

    // If no platforms, fall back to ground level (should not happen)
    if (platforms.empty())
    {
        GeneratedObject groundPlatform;
        groundPlatform.x = 0;
        groundPlatform.y = GROUND_Y;
        groundPlatform.width = LEVEL_WIDTH;
        groundPlatform.height = 50;
        platforms.push_back(groundPlatform);
    }

    while (enemyCount < MAX_ENEMIES)
    {
        GeneratedObject enemy;
        enemy.width = 50;
        enemy.height = 50;

        // Choose a random platform
        int platformIndex = randomInt(0, platforms.size() - 1);
        const GeneratedObject& platform = platforms[platformIndex];

        // Place enemy on platform, ensuring enemy is within platform bounds
        enemy.x = randomFloat(platform.x + 10, platform.x + platform.width - enemy.width - 10);
        enemy.y = platform.y - enemy.height;

        // Ensure enemy is not in camera view (x > CAMERA_WIDTH + 100)
        if (enemy.x <= CAMERA_WIDTH + 100)
        {
            // Shift to the right side
            enemy.x = randomFloat(CAMERA_WIDTH + 100, LEVEL_WIDTH - 100);
            // Find a platform near this x (simple: choose random platform again)
            // For simplicity, keep the same platform but adjust x if possible
            if (enemy.x < platform.x || enemy.x > platform.x + platform.width - enemy.width)
            {
                // Choose another platform
                platformIndex = randomInt(0, platforms.size() - 1);
                const GeneratedObject& newPlatform = platforms[platformIndex];
                enemy.x = randomFloat(newPlatform.x + 10, newPlatform.x + newPlatform.width - enemy.width - 10);
                enemy.y = newPlatform.y - enemy.height;
            }
        }

        enemy.type = "Shooter";

        objects.push_back(enemy);
        shooters.push_back(enemy);
        enemyCount++;
    }
}

int LevelGenerator::findPlatformAtX(const std::vector<GeneratedObject>& platforms, float x)
{
    // Find the platform that contains the given x coordinate (within platform bounds)
    for (size_t i = 0; i < platforms.size(); ++i)
    {
        const auto& platform = platforms[i];
        if (x >= platform.x && x <= platform.x + platform.width)
        {
            return i;
        }
    }
    // If none found, return -1
    return -1;
}

void LevelGenerator::generateTrees(std::vector<GeneratedObject>& objects)
{
    // At most one tree per TREE_SEGMENT_WIDTH pixels of level width.
    // The trunk stays at least TREE_RIGHT_MARGIN pixels away from the right edge
    // of its segment, so leaves and branches have room (they may extend further right).
    const int TREE_SEGMENT_WIDTH = 500;
    const int TREE_RIGHT_MARGIN = 200;
    int numTrees = LEVEL_WIDTH / TREE_SEGMENT_WIDTH;

    const int TRUNK_HEIGHT_MIN = 100;
    const int TRUNK_HEIGHT_MAX = 200;
    const int BRANCHES_MIN = 2;
    const int BRANCHES_MAX = 4;
    const int MIN_BRANCH_LENGTH = 2; // tiles
    const int MAX_BRANCH_LENGTH = 6; // tiles

    for (int i = 0; i < numTrees; i++)
    {
        // Trunk x within its segment, at least TREE_RIGHT_MARGIN from the segment's right edge
        int segmentStart = i * TREE_SEGMENT_WIDTH;
        float x = randomFloat(segmentStart, segmentStart + TREE_SEGMENT_WIDTH - TREE_RIGHT_MARGIN);
        float y = GROUND_Y; // place on ground level

        int trunkHeight = randomInt(TRUNK_HEIGHT_MIN, TRUNK_HEIGHT_MAX);
        int numBranches = randomInt(BRANCHES_MIN, BRANCHES_MAX);

        // Add tree
        GeneratedObject treeObj;
        treeObj.x = x;
        treeObj.y = y;
        treeObj.width = 0;
        treeObj.height = 0;
        treeObj.type = "Tree";
        treeObj.trunkHeight = trunkHeight;
        treeObj.numBranches = numBranches;
        treeObj.minBranchLength = MIN_BRANCH_LENGTH;
        treeObj.maxBranchLength = MAX_BRANCH_LENGTH;
        objects.push_back(treeObj);
    }
}

float LevelGenerator::randomFloat(float min, float max)
{
    return min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

int LevelGenerator::randomInt(int min, int max)
{
    return min + std::rand() % (max - min + 1);
}
