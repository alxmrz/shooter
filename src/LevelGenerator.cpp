#include <cstdlib>
#include <ctime>
#include <climits>
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
#include "objects/interactive/Flag.h"
#include "objects/interactive/Aim.h"
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
constexpr int LevelGenerator::ENEMY_WIDTH;
constexpr int LevelGenerator::ENEMY_HEIGHT;
constexpr int LevelGenerator::ENEMY_CRYSTAL_STEP;
constexpr int LevelGenerator::CAMERA_SAFE_X;
constexpr int LevelGenerator::TREE_HEIGHT;
constexpr int LevelGenerator::TREE_TRUNK_HEIGHT;
constexpr int LevelGenerator::TREE_BRANCH_STEP;
constexpr int LevelGenerator::TREE_TRUNK_MARGIN;
constexpr int LevelGenerator::TREE_COLUMN_STEP;
constexpr int LevelGenerator::FLAG_SIZE;

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

    generateFlag(objects);


    // Create the player on top of the fixed start block in the bottom-left corner.
    const int startBlockCenterX = START_BLOCK_X + (START_BLOCK_WIDTH - PLAYER_WIDTH) / 2;
    Shooter* player = go->fabric->createShooter(
        startBlockCenterX, START_BLOCK_Y - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT);
    player->setMain(true);
    go->player = player;
    go->all.push_back(player);

    go->aim = go->fabric->createAim(startBlockCenterX, START_BLOCK_Y - PLAYER_HEIGHT, 10, 10);
    go->all.push_back(go->aim);

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
                                                obj.maxBranchLength, obj.branchStep, obj.topBranchSide,
                                                obj.topBranchLength);
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
        else if (obj.type == "Flag")
        {
            go->all.push_back(go->fabric->createFlag(obj.x, obj.y, obj.width, obj.height));
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
    const int MIN_Y = 50;

    // Width multiplier applied to every generated block.
    const float WIDTH_SCALE_MIN = 1.2f;
    const float WIDTH_SCALE_MAX = 2.0f;

    // Spatial hash grid: only blocks in nearby cells are checked for clearance,
    // so the number of proximity checks stays linear in the number of blocks.
    SpacingGrid grid(CELL_SIZE, static_cast<float>(HALF_GAP));

    auto boxOf = [](const GeneratedObject& block) -> SpacingBox
    {
        return {block.x, block.y, block.width, block.height};
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

    // Fixed start block in the bottom-left corner, on the ground.
    GeneratedObject startBlock;
    startBlock.x = START_BLOCK_X;
    startBlock.y = START_BLOCK_Y;
    startBlock.width = START_BLOCK_WIDTH;
    startBlock.height = START_BLOCK_HEIGHT;
    startBlock.type = "GroundBlock";
    grid.insert(boxOf(startBlock));
    objects.push_back(startBlock);

    // Blocks that still need an upward neighbour, with their ground index (used
    // to decide which of them carry a tree). Generation is bottom-up: every
    // block spawns exactly one neighbour above it, so a block that will carry a
    // tree reserves a full TREE_HEIGHT of room without moving anything.
    struct Node
    {
        GeneratedObject block;
        int index;
        size_t objectIndex;
    };
    std::vector<Node> frontier;
    frontier.push_back({startBlock, 0, objects.size() - 1});
    int groundCount = 1;

    auto placeUpward = [&](const Node& lower, bool carriesTree, int forcedSide,
                           GeneratedObject& out) -> bool
    {
        if (lower.block.y <= MIN_Y)
        {
            return false;
        }

        // A tree-carrying block leaves a full tree height of room; the tree's
        // branches bridge the gap. Other blocks keep a jump-sized step.
        const int step = carriesTree ? TREE_HEIGHT : randomInt(50, 100);
        const int y = static_cast<int>(lower.block.y) - step;
        if (y < MIN_Y)
        {
            return false;
        }

        const int ATTEMPTS = 40;
        for (int attempt = 0; attempt < ATTEMPTS; ++attempt)
        {
            const int width = generateBlockWidth();
            const int gap = randomInt(MIN_BLOCK_GAP, MAX_HORIZONTAL_REACH);
            const bool left = (forcedSide >= 0)
                ? (forcedSide == 0)
                : (randomFloat(0, 1) < 0.5f);
            const float x = left
                ? lower.block.x - static_cast<float>(gap) - width
                : lower.block.x + lower.block.width + static_cast<float>(gap);

            if (x < 50.f || x + width > LEVEL_WIDTH - 50.f)
            {
                continue;
            }

            GeneratedObject candidate;
            candidate.x = x;
            candidate.y = static_cast<float>(y);
            candidate.width = width;
            candidate.height = BLOCK_HEIGHT;
            candidate.type = "GroundBlock";

            if (!grid.canPlace(boxOf(candidate)))
            {
                continue;
            }

            grid.insert(boxOf(candidate));
            out = candidate;
            return true;
        }
        return false;
    };

    // Places the upward neighbour of `lower`. A block that carries a tree puts
    // its neighbour on the side that will hold the tree's topmost branch, so the
    // player can climb the tree straight onto that platform.
    auto placeForLower = [&](const Node& lower, GeneratedObject& out, int& side) -> bool
    {
        const bool carriesTree = isTreeCandidate(lower.block, lower.index);
        if (!carriesTree)
        {
            side = -1;
            return placeUpward(lower, false, -1, out);
        }

        const int preferred = (randomFloat(0, 1) < 0.5f) ? 0 : 1;
        if (placeUpward(lower, true, preferred, out))
        {
            side = preferred;
            return true;
        }
        if (placeUpward(lower, true, 1 - preferred, out))
        {
            side = 1 - preferred;
            return true;
        }
        side = -1;
        return false;
    };

    int guard = 0;
    const int GUARD_LIMIT = 4000;
    while (!frontier.empty() && guard < GUARD_LIMIT)
    {
        ++guard;

        std::vector<Node> next;
        for (const Node& lower : frontier)
        {
            GeneratedObject candidate;
            int side = -1;
            if (placeForLower(lower, candidate, side))
            {
                if (side >= 0)
                {
                    objects[lower.objectIndex].topBranchSide = side;
                }
                objects.push_back(candidate);
                next.push_back({candidate, groundCount, objects.size() - 1});
                ++groundCount;
            }
        }
        frontier.swap(next);
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
    // One enemy on every second crystal-carrying ground block (in creation
    // order), placed on its own platform. Platforms with a tree carry no
    // crystal and are therefore skipped. The crystal ordinal is deterministic,
    // so no enemy lands in mid-air and no two enemies share a platform.
    const std::vector<size_t> indices = groundBlockIndices(objects);
    int crystalOrdinal = 0;

    for (size_t k = 0; k < indices.size(); ++k)
    {
        const GeneratedObject& platform = objects[indices[k]];
        const bool hasCrystal = platformHasCrystal(objects, platform, k);
        crystalOrdinal += hasCrystal ? 1 : 0;

        if (shouldPlaceEnemy(platform, hasCrystal, crystalOrdinal))
        {
            objects.push_back(makeEnemy(platform));
        }
    }
}

bool LevelGenerator::platformHasCrystal(const std::vector<GeneratedObject>& objects,
                                        const GeneratedObject& block, size_t index) const
{
    return (effectiveItemMask(objects, block, index) & ITEM_CRYSTAL) != 0;
}

bool LevelGenerator::isEverySecondCrystalPlatform(int crystalOrdinal)
{
    return crystalOrdinal > 0 && crystalOrdinal % ENEMY_CRYSTAL_STEP == 0;
}

bool LevelGenerator::shouldPlaceEnemy(const GeneratedObject& platform, bool hasCrystal,
                                      int crystalOrdinal)
{
    return hasCrystal
        && isEverySecondCrystalPlatform(crystalOrdinal)
        && isEnemyOutsideCamera(platform);
}

bool LevelGenerator::isEnemyOutsideCamera(const GeneratedObject& platform)
{
    return enemySpawnX(platform) >= static_cast<float>(CAMERA_SAFE_X);
}

float LevelGenerator::enemySpawnX(const GeneratedObject& platform)
{
    return platform.x + (platform.width - ENEMY_WIDTH) / 2.0f;
}

GeneratedObject LevelGenerator::makeEnemy(const GeneratedObject& platform)
{
    GeneratedObject enemy;
    enemy.x = enemySpawnX(platform);
    enemy.y = platform.y - ENEMY_HEIGHT;
    enemy.width = ENEMY_WIDTH;
    enemy.height = ENEMY_HEIGHT;
    enemy.type = "Shooter";
    return enemy;
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

bool LevelGenerator::blocksOverhead(const GeneratedObject& candidate,
                                    const GeneratedObject& block, float trunkX,
                                    int trunkWidth)
{
    const bool above = candidate.y + candidate.height <= block.y;
    const bool overlapsX = trunkX < candidate.x + candidate.width &&
                           trunkX + trunkWidth > candidate.x;
    return above && overlapsX;
}

std::vector<size_t> LevelGenerator::groundBlockIndices(
    const std::vector<GeneratedObject>& objects)
{
    std::vector<size_t> indices;
    for (size_t i = 0; i < objects.size(); ++i)
    {
        if (objects[i].type == "GroundBlock")
        {
            indices.push_back(i);
        }
    }
    return indices;
}

int LevelGenerator::clearanceAtX(const std::vector<GeneratedObject>& objects,
                                 const GeneratedObject& block, float trunkX,
                                 int trunkWidth)
{
    int worst = INT_MAX;
    for (const auto& candidate : objects)
    {
        if (!blocksOverhead(candidate, block, trunkX, trunkWidth))
        {
            continue;
        }
        const int clearance = static_cast<int>(block.y) -
                              (static_cast<int>(candidate.y) + candidate.height);
        if (clearance < worst)
        {
            worst = clearance;
        }
    }
    return worst;
}

std::vector<float> LevelGenerator::clearTrunkPositions(
    const std::vector<GeneratedObject>& objects, const GeneratedObject& block)
{
    std::vector<float> positions;
    const float from = block.x + TREE_TRUNK_MARGIN;
    const float to = block.x + block.width - TREE_TRUNK_MARGIN;
    for (float x = from; x <= to; x += TREE_COLUMN_STEP)
    {
        if (clearanceAtX(objects, block, x, Tree::TRUNK_WIDTH) >= TREE_HEIGHT)
        {
            positions.push_back(x);
        }
    }
    return positions;
}

bool LevelGenerator::isTreeCandidate(const GeneratedObject& block, size_t index)
{
    const int PLATFORM_INTERVAL = 3;
    return (index + 1) % static_cast<size_t>(PLATFORM_INTERVAL) == 0 &&
           block.width >= TREE_MIN_BLOCK_WIDTH;
}

bool LevelGenerator::buildTree(std::vector<GeneratedObject>& objects, size_t blockIndex)
{
    const int MIN_BRANCH_LENGTH = 2; // tiles
    const int MAX_BRANCH_LENGTH = 6; // tiles

    const GeneratedObject block = objects[blockIndex];

    // Choose a trunk position with a clear TREE_HEIGHT column overhead; if the
    // whole block is blocked, no tree is placed here.
    const std::vector<float> positions = clearTrunkPositions(objects, block);
    if (positions.empty())
    {
        return false;
    }
    const float x = positions[randomInt(0, static_cast<int>(positions.size()) - 1)];

    GeneratedObject treeObj;
    treeObj.x = x;
    treeObj.y = block.y; // trunk base on the block's top edge
    treeObj.width = 0;
    treeObj.height = 0;
    treeObj.type = "Tree";
    treeObj.trunkHeight = TREE_TRUNK_HEIGHT;
    treeObj.minBranchLength = MIN_BRANCH_LENGTH;
    treeObj.maxBranchLength = MAX_BRANCH_LENGTH;
    treeObj.branchStep = TREE_BRANCH_STEP;
    treeObj.topBranchSide = block.topBranchSide;
    treeObj.topBranchLength = topBranchLengthFor(block, x, block.topBranchSide);
    objects.push_back(treeObj);
    return true;
}

int LevelGenerator::topBranchLengthFor(const GeneratedObject& block, float trunkX, int side)
{
    // The top branch base sits at the far trunk edge and reaches the host block
    // edge on the branch side, so the player can walk out to the edge and jump
    // across the reserved gap to the platform above.
    const float outerEnd = (side == 0)
        ? trunkX + Tree::TRUNK_WIDTH
        : block.x + block.width;
    const float innerEnd = (side == 0) ? block.x : trunkX;
    return static_cast<int>(outerEnd - innerEnd);
}

void LevelGenerator::generateTrees(std::vector<GeneratedObject>& objects)
{
    // Every third wide ground block grows a fixed-height (TREE_HEIGHT) tree with
    // evenly spaced step branches so the player can climb the tree itself. The
    // trunk is placed where the vertical column is clear; blocked blocks are
    // skipped so no ground block is moved.
    const std::vector<size_t> indices = groundBlockIndices(objects);

    for (size_t k = 0; k < indices.size(); ++k)
    {
        if (isTreeCandidate(objects[indices[k]], k))
        {
            buildTree(objects, indices[k]);
        }
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

void LevelGenerator::generateFlag(std::vector<GeneratedObject>& objects)
{
    const GeneratedObject* top = highestGroundBlock(objects);
    if (top == nullptr)
    {
        return;
    }

    objects.push_back(makeFlag(*top));
}

const GeneratedObject* LevelGenerator::highestGroundBlock(
    const std::vector<GeneratedObject>& objects)
{
    std::vector<const GeneratedObject*> blocks;
    addGroundBlocks(blocks, objects);
    if (blocks.empty())
    {
        return nullptr;
    }

    return *std::min_element(blocks.begin(), blocks.end(), isAbove);
}

void LevelGenerator::addGroundBlocks(std::vector<const GeneratedObject*>& out,
                                     const std::vector<GeneratedObject>& objects)
{
    for (const auto& obj : objects)
    {
        addIfGroundBlock(out, obj);
    }
}

void LevelGenerator::addIfGroundBlock(std::vector<const GeneratedObject*>& out,
                                      const GeneratedObject& object)
{
    if (object.type != "GroundBlock")
    {
        return;
    }

    out.push_back(&object);
}

bool LevelGenerator::isAbove(const GeneratedObject* a, const GeneratedObject* b)
{
    if (a->y != b->y)
    {
        return a->y < b->y;
    }

    return a->x < b->x;
}

GeneratedObject LevelGenerator::makeFlag(const GeneratedObject& block)
{
    GeneratedObject flag;
    flag.x = block.x + (block.width - FLAG_SIZE) / 2.0f;
    flag.y = block.y - FLAG_SIZE;
    flag.width = FLAG_SIZE;
    flag.height = FLAG_SIZE;
    flag.type = "Flag";
    return flag;
}
