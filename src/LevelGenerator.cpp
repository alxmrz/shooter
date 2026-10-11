#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <vector>
#include "LevelGenerator.h"
#include "SpacingGrid.h"
#include "Map.h"
#include "GameObjects.h"
#include "Fabric.h"
#include "objects/units/Shooter.h"
#include "objects/interactive/Platform.h"
#include "objects/interactive/Heart.h"
#include "objects/interactive/Ammunition.h"
#include "objects/interactive/Crystal.h"
#include "objects/interactive/Tree.h"
#include "objects/backgrounds/GroundBlock.h"

constexpr int LevelGenerator::LEVEL_WIDTH;
constexpr int LevelGenerator::LEVEL_HEIGHT;
constexpr int LevelGenerator::GROUND_Y;
constexpr int LevelGenerator::MAX_ENEMIES;
constexpr int LevelGenerator::CAMERA_WIDTH;
constexpr int LevelGenerator::CAMERA_HEIGHT;
constexpr int LevelGenerator::PLAYER_WIDTH;
constexpr int LevelGenerator::PLAYER_HEIGHT;
constexpr int LevelGenerator::GROUND_JUMP_HEIGHT;
constexpr int LevelGenerator::AIR_JUMP_HEIGHT;
constexpr int LevelGenerator::DOUBLE_JUMP_HEIGHT;
constexpr int LevelGenerator::MAX_JUMP_HEIGHT;
constexpr int LevelGenerator::START_BLOCK_X;
constexpr int LevelGenerator::START_BLOCK_Y;
constexpr int LevelGenerator::START_BLOCK_WIDTH;
constexpr int LevelGenerator::START_BLOCK_HEIGHT;
constexpr int LevelGenerator::TREE_MIN_BLOCK_WIDTH;

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

    generatePlatformItems(objects);

    generateEnemies(objects);


    // Create the player on top of the fixed start block in the bottom-left corner.
    const int startBlockCenterX = START_BLOCK_X + (START_BLOCK_WIDTH - PLAYER_WIDTH) / 2;
    Shooter* player = go->fabric->createShooter(
        startBlockCenterX, START_BLOCK_Y - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT);
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
        else if (obj.type == "Crystal")
        {
            go->all.push_back(go->fabric->createCrystal(obj.x, obj.y, obj.width, obj.height));
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
    // Ground blocks with varying lengths, widened by a random factor.
    // Tile width assumed to be 50 pixels (same as ground height).
    const int TILE_WIDTH = 50;
    const int BLOCK_HEIGHT = 25;

    // Minimum clearance between any two ground blocks: 2 widths / 2 heights of
    // the Shooter (50x50) => 100 pixels along at least one axis.
    const int MIN_BLOCK_GAP = 2 * PLAYER_WIDTH;
    const int HALF_GAP = MIN_BLOCK_GAP / 2;
    const int CELL_SIZE = MIN_BLOCK_GAP;

    // Width multiplier applied to every generated block.
    const float WIDTH_SCALE_MIN = 1.2f;
    const float WIDTH_SCALE_MAX = 2.0f;

    // Spatial hash grid: only blocks in nearby cells are checked for clearance,
    // so the number of proximity checks stays linear in the number of blocks.
    SpacingGrid grid(CELL_SIZE, static_cast<float>(HALF_GAP));

    auto overlapsExisting = [&](const GeneratedObject& candidate) -> bool
    {
        SpacingBox box = {candidate.x, candidate.y, candidate.width, candidate.height};
        return !grid.canPlace(box);
    };

    auto insertIntoGrid = [&](const GeneratedObject& block)
    {
        SpacingBox box = {block.x, block.y, block.width, block.height};
        grid.insert(box);
    };

    auto generateBlockWidth = [&]() -> int
    {
        // Base length 3-10 tiles, more long than short, then widened 1.2-2x.
        int baseTiles = (randomFloat(0, 1) < 0.6) ? randomInt(6, 10) : randomInt(3, 5);
        float scale = randomFloat(WIDTH_SCALE_MIN, WIDTH_SCALE_MAX);
        int tiles = static_cast<int>(baseTiles * scale + 0.5f);
        if (tiles < 3)
        {
            tiles = 3;
        }
        return tiles * TILE_WIDTH;
    };

    // Horizontal gap between two boxes; a negative value means they overlap.
    auto horizontalGap = [](const GeneratedObject& a, const GeneratedObject& b) -> float
    {
        float aRight = a.x + a.width;
        float bRight = b.x + b.width;
        return std::max(a.x, b.x) - std::min(aRight, bRight);
    };

    // Candidate horizontal gaps between adjacent levels. The order is shuffled
    // per placement so the spacing is dynamic: sometimes the minimum block gap,
    // sometimes up to 50% wider, but always within jump reach.
    std::vector<int> gapCandidates;
    for (int gap = MIN_BLOCK_GAP; gap <= MAX_HORIZONTAL_REACH; gap += 10)
    {
        gapCandidates.push_back(gap);
    }

    // Fixed start block in the bottom-left corner, on the ground.
    GeneratedObject startBlock;
    startBlock.x = START_BLOCK_X;
    startBlock.y = START_BLOCK_Y;
    startBlock.width = START_BLOCK_WIDTH;
    startBlock.height = START_BLOCK_HEIGHT;
    startBlock.type = "GroundBlock";
    insertIntoGrid(startBlock);
    objects.push_back(startBlock);

    // Blocks of the level directly below the one being generated. Every one of
    // them needs a reachable neighbour on the next level up.
    std::vector<GeneratedObject> lower;
    lower.push_back(startBlock);

    // Start above ground and move upward.
    int currentY = GROUND_Y - 100;

    while (currentY > 100 && !lower.empty())
    {
        std::vector<GeneratedObject> current;

        // Indices into `lower` that still need a reachable upward neighbour.
        std::vector<size_t> pending;
        for (size_t i = 0; i < lower.size(); ++i)
        {
            pending.push_back(i);
        }

        // Place blocks until every lower block is covered by a neighbour within
        // MAX_HORIZONTAL_REACH. A single block may cover several lower blocks.
        int guard = 0;
        const int GUARD_LIMIT = static_cast<int>(lower.size()) * 20 + 40;
        while (!pending.empty() && guard < GUARD_LIMIT)
        {
            ++guard;

            size_t pick = static_cast<size_t>(randomInt(0, static_cast<int>(pending.size()) - 1));
            const GeneratedObject& source = lower[pending[pick]];

            bool placed = false;
            bool leftFirst = randomFloat(0, 1) < 0.5f;

            // Randomise the gap order so the chosen spacing varies between
            // attempts (sometimes minimal, sometimes up to 50% wider).
            std::vector<int> gaps = gapCandidates;
            for (size_t i = gaps.size(); i > 1; --i)
            {
                int j = randomInt(0, static_cast<int>(i) - 1);
                std::swap(gaps[i - 1], gaps[j]);
            }

            // Try both sides and every jump-reachable gap.
            for (int side = 0; side < 2 && !placed; ++side)
            {
                bool placeLeft = (side == 0) ? leftFirst : !leftFirst;

                for (int gap : gaps)
                {
                    if (placed)
                    {
                        break;
                    }

                    int blockWidth = generateBlockWidth();

                    float x = placeLeft
                        ? source.x - static_cast<float>(gap) - blockWidth
                        : source.x + source.width + static_cast<float>(gap);

                    // Keep the block inside the level borders (50 pixels wide).
                    if (x < 50.f || x + blockWidth > LEVEL_WIDTH - 50.f)
                    {
                        continue;
                    }

                    GeneratedObject candidate;
                    candidate.x = x;
                    candidate.y = currentY;
                    candidate.width = blockWidth;
                    candidate.height = BLOCK_HEIGHT;
                    candidate.type = "GroundBlock";

                    if (overlapsExisting(candidate))
                    {
                        continue;
                    }

                    insertIntoGrid(candidate);
                    objects.push_back(candidate);
                    current.push_back(candidate);
                    placed = true;

                    // Drop every lower block this candidate now covers.
                    std::vector<size_t> remaining;
                    for (size_t idx : pending)
                    {
                        float g = horizontalGap(candidate, lower[idx]);
                        if (g < static_cast<float>(MIN_BLOCK_GAP) ||
                            g > static_cast<float>(MAX_HORIZONTAL_REACH))
                        {
                            remaining.push_back(idx);
                        }
                    }
                    pending.swap(remaining);
                }
            }
        }

        lower = current;

        // Move up by a random step not exceeding the maximum vertical distance.
        int step = randomInt(50, 100);
        if (step > MAX_JUMP_HEIGHT)
        {
            step = MAX_JUMP_HEIGHT;
        }
        currentY -= step;
    }
}

bool LevelGenerator::isNthPlatform(size_t index, int step)
{
    return (index + 1) % static_cast<size_t>(step) == 0;
}

bool LevelGenerator::platformHasTree(const std::vector<GeneratedObject>& objects,
                                     const GeneratedObject& block)
{
    for (const auto& obj : objects)
    {
        if (obj.type == "Tree" && obj.y == block.y &&
            obj.x >= block.x && obj.x <= block.x + block.width)
        {
            return true;
        }
    }
    return false;
}

std::vector<GeneratedObject> LevelGenerator::groundBlocks(
    const std::vector<GeneratedObject>& objects)
{
    std::vector<GeneratedObject> blocks;
    for (const auto& obj : objects)
    {
        if (obj.type == "GroundBlock")
        {
            blocks.push_back(obj);
        }
    }
    return blocks;
}

int LevelGenerator::itemMaskForIndex(size_t index)
{
    const int ammo = static_cast<int>(isNthPlatform(index, AMMUNITION_STEP));
    const int heart = static_cast<int>(isNthPlatform(index, HEART_STEP));
    const int crystal = static_cast<int>(isNthPlatform(index, CRYSTAL_STEP));
    return ammo * ITEM_AMMO + heart * ITEM_HEART + crystal * ITEM_CRYSTAL;
}

int LevelGenerator::popcount3(int mask)
{
    return (mask & 1) + ((mask >> 1) & 1) + ((mask >> 2) & 1);
}

int LevelGenerator::slotForItem(int mask, int item)
{
    return popcount3(mask & (item - 1));
}

int LevelGenerator::effectiveItemMask(const std::vector<GeneratedObject>& objects,
                                      const GeneratedObject& block, size_t index) const
{
    if (platformHasTree(objects, block))
    {
        return 0;
    }
    return itemMaskForIndex(index);
}

float LevelGenerator::itemSlotX(const GeneratedObject& block, int slot, int count)
{
    if (count <= 1)
    {
        return block.x + (block.width - ITEM_SIZE) / 2.0f;
    }
    return block.x + static_cast<float>(slot) * (block.width - ITEM_SIZE) /
        static_cast<float>(count - 1);
}

void LevelGenerator::placeAmmunition(std::vector<GeneratedObject>& objects,
                                     const GeneratedObject& block, float x)
{
    GeneratedObject item;
    item.x = x;
    item.y = block.y - ITEM_SIZE;
    item.width = ITEM_SIZE;
    item.height = ITEM_SIZE;
    item.type = "Ammunition";
    objects.push_back(item);
}

void LevelGenerator::placeHeart(std::vector<GeneratedObject>& objects,
                                const GeneratedObject& block, float x)
{
    GeneratedObject item;
    item.x = x;
    item.y = block.y - ITEM_SIZE;
    item.width = ITEM_SIZE;
    item.height = ITEM_SIZE;
    item.type = "Heart";
    objects.push_back(item);
}

void LevelGenerator::placeCrystal(std::vector<GeneratedObject>& objects,
                                  const GeneratedObject& block, float x)
{
    GeneratedObject item;
    item.x = x;
    item.y = block.y - ITEM_SIZE;
    item.width = ITEM_SIZE;
    item.height = ITEM_SIZE;
    item.type = "Crystal";
    objects.push_back(item);
}

void LevelGenerator::generateGuaranteedAmmo(std::vector<GeneratedObject>& objects) const
{
    const std::vector<GeneratedObject> blocks = groundBlocks(objects);
    for (size_t i = 0; i < blocks.size(); ++i)
    {
        const int mask = effectiveItemMask(objects, blocks[i], i);
        if (mask & ITEM_AMMO)
        {
            const int count = popcount3(mask);
            const int slot = slotForItem(mask, ITEM_AMMO);
            placeAmmunition(objects, blocks[i], itemSlotX(blocks[i], slot, count));
        }
    }
}

void LevelGenerator::generateGuaranteedHeart(std::vector<GeneratedObject>& objects) const
{
    const std::vector<GeneratedObject> blocks = groundBlocks(objects);
    for (size_t i = 0; i < blocks.size(); ++i)
    {
        const int mask = effectiveItemMask(objects, blocks[i], i);
        if (mask & ITEM_HEART)
        {
            const int count = popcount3(mask);
            const int slot = slotForItem(mask, ITEM_HEART);
            placeHeart(objects, blocks[i], itemSlotX(blocks[i], slot, count));
        }
    }
}

void LevelGenerator::generateGuaranteedCrystal(std::vector<GeneratedObject>& objects) const
{
    const std::vector<GeneratedObject> blocks = groundBlocks(objects);
    for (size_t i = 0; i < blocks.size(); ++i)
    {
        const int mask = effectiveItemMask(objects, blocks[i], i);
        if (mask & ITEM_CRYSTAL)
        {
            const int count = popcount3(mask);
            const int slot = slotForItem(mask, ITEM_CRYSTAL);
            placeCrystal(objects, blocks[i], itemSlotX(blocks[i], slot, count));
        }
    }
}

void LevelGenerator::generatePlatformItems(std::vector<GeneratedObject>& objects)
{
    generateGuaranteedAmmo(objects);
    generateGuaranteedHeart(objects);
    generateGuaranteedCrystal(objects);
}

void LevelGenerator::generateEnemies(std::vector<GeneratedObject>& objects)
{
    // Enemies are placed on ground blocks, at most one per every tenth platform.
    const int PLATFORM_INTERVAL = 10;
    const int ENEMY_WIDTH = 50;
    const int ENEMY_HEIGHT = 50;
    const int EDGE_MARGIN = 10;
    const float CAMERA_SAFE_X = static_cast<float>(CAMERA_WIDTH + 100);

    // Platforms are the generated ground blocks (including the start block).
    std::vector<GeneratedObject> platforms;
    for (const auto& obj : objects)
    {
        if (obj.type == "GroundBlock")
        {
            platforms.push_back(obj);
        }
    }

    int enemyCount = 0;

    for (size_t i = 0; i < platforms.size() && enemyCount < MAX_ENEMIES; ++i)
    {
        // At most one enemy per PLATFORM_INTERVAL platforms: only every tenth.
        if ((i + 1) % PLATFORM_INTERVAL != 0)
        {
            continue;
        }

        const GeneratedObject& platform = platforms[i];

        float minX = platform.x + EDGE_MARGIN;
        float maxX = platform.x + platform.width - ENEMY_WIDTH - EDGE_MARGIN;
        if (maxX < minX)
        {
            continue; // platform too narrow for an enemy
        }

        // Keep the enemy outside the camera view.
        float spawnMinX = std::max(minX, CAMERA_SAFE_X);
        if (spawnMinX > maxX)
        {
            continue; // whole platform is inside the camera view
        }

        GeneratedObject enemy;
        enemy.x = randomFloat(spawnMinX, maxX);
        enemy.y = platform.y - ENEMY_HEIGHT;
        enemy.width = ENEMY_WIDTH;
        enemy.height = ENEMY_HEIGHT;
        enemy.type = "Shooter";

        objects.push_back(enemy);
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
    // A tree appears on every third ground block at most. The block must also be
    // wide enough; the trunk stands on its top edge and stays inside its bounds.
    const int TRUNK_HEIGHT_MIN = 100;
    const int TRUNK_HEIGHT_MAX = 200;
    const int BRANCHES_MIN = 2;
    const int BRANCHES_MAX = 4;
    const int MIN_BRANCH_LENGTH = 2; // tiles
    const int MAX_BRANCH_LENGTH = 6; // tiles
    const int TRUNK_MARGIN = 100;
    const int PLATFORM_INTERVAL = 3;

    // Snapshot blocks first: trees are appended to the same vector.
    std::vector<GeneratedObject> blocks;
    for (const auto& obj : objects)
    {
        if (obj.type == "GroundBlock")
        {
            blocks.push_back(obj);
        }
    }

    for (size_t i = 0; i < blocks.size(); ++i)
    {
        // At most one tree per PLATFORM_INTERVAL blocks: only every third block.
        if ((i + 1) % PLATFORM_INTERVAL != 0)
        {
            continue;
        }
        if (blocks[i].width < TREE_MIN_BLOCK_WIDTH)
        {
            continue;
        }

        const GeneratedObject& block = blocks[i];

        // Trunk x within the block, leaving room on both sides.
        float x = randomFloat(block.x + TRUNK_MARGIN, block.x + block.width - TRUNK_MARGIN);
        float y = block.y; // trunk base on the block's top edge

        GeneratedObject treeObj;
        treeObj.x = x;
        treeObj.y = y;
        treeObj.width = 0;
        treeObj.height = 0;
        treeObj.type = "Tree";
        treeObj.trunkHeight = randomInt(TRUNK_HEIGHT_MIN, TRUNK_HEIGHT_MAX);
        treeObj.numBranches = randomInt(BRANCHES_MIN, BRANCHES_MAX);
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
