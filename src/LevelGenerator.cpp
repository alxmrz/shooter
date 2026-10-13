#include <cstdlib>
#include <ctime>
#include <climits>
#include <algorithm>
#include <iterator>
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

namespace {
const int PLATFORM_TILE_WIDTH = 50;
const int PLATFORM_BLOCK_HEIGHT = 25;
const int MIN_BLOCK_GAP = 2 * LevelGenerator::PLAYER_WIDTH;
const int HALF_GAP = MIN_BLOCK_GAP / 2;
const int SPACING_CELL_SIZE = MIN_BLOCK_GAP;
const int MIN_PLATFORM_Y = 50;
const float WIDTH_SCALE_MIN = 1.2f;
const float WIDTH_SCALE_MAX = 2.0f;
const int UPWARD_ATTEMPTS = 40;
const int FRONTIER_GUARD_LIMIT = 4000;
const int TREE_PLATFORM_INTERVAL = 3;
const int TREE_MIN_BRANCH_LENGTH = 2;
const int TREE_MAX_BRANCH_LENGTH = 6;
const float LEVEL_EDGE_MARGIN = 50.f;

SpacingBox spacingBoxOf(const GeneratedObject& block) {
    return {block.x, block.y, block.width, block.height};
}

bool isTreeOnBlock(const GeneratedObject& object, const GeneratedObject& block) {
    return object.type == "Tree" && object.y == block.y &&
           object.x >= block.x && object.x <= block.x + block.width;
}

struct TreeOnBlock {
    const GeneratedObject& block;

    bool operator()(const GeneratedObject& object) const {
        return isTreeOnBlock(object, block);
    }
};
}

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

    createPlayer();

    instantiateObjects(objects);
}

void LevelGenerator::createPlayer()
{
    // Create the player on top of the fixed start block in the bottom-left corner.
    const int startBlockCenterX = START_BLOCK_X + (START_BLOCK_WIDTH - PLAYER_WIDTH) / 2;
    Shooter* player = go->fabric->createShooter(
        startBlockCenterX, START_BLOCK_Y - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT);
    player->setMain(true);
    go->player = player;
    go->all.push_back(player);

    go->aim = go->fabric->createAim(startBlockCenterX, START_BLOCK_Y - PLAYER_HEIGHT, 10, 10);
    go->all.push_back(go->aim);
}

void LevelGenerator::instantiateObjects(const std::vector<GeneratedObject>& objects)
{
    for (const auto& obj : objects)
    {
        instantiateObject(obj);
    }
}

void LevelGenerator::instantiateObject(const GeneratedObject& obj)
{
    ObjectCreator creator = creatorFor(obj.type);

    if (creator == nullptr)
    {
        return;
    }

    (this->*creator)(obj);
}

LevelGenerator::ObjectCreator LevelGenerator::creatorFor(const std::string& type)
{
    const CreatorTable& table = creatorTable();
    CreatorTable::const_iterator it = table.find(type);

    if (it == table.end())
    {
        return nullptr;
    }

    return it->second;
}

const LevelGenerator::CreatorTable& LevelGenerator::creatorTable()
{
    static const CreatorTable table = {
        {"Border", &LevelGenerator::createBorderObject},
        {"Platform", &LevelGenerator::createPlatformObject},
        {"GroundBlock", &LevelGenerator::createGroundBlockObject},
        {"Tree", &LevelGenerator::createTreeObject},
        {"Heart", &LevelGenerator::createHeartObject},
        {"Ammunition", &LevelGenerator::createAmmoObject},
        {"Crystal", &LevelGenerator::createCrystalObject},
        {"Shooter", &LevelGenerator::createShooterObject},
        {"Flag", &LevelGenerator::createFlagObject}
    };
    return table;
}

void LevelGenerator::createBorderObject(const GeneratedObject& obj)
{
    go->borders.insert({
        static_cast<int>(obj.x),
        go->fabric->createBorder(obj.x, obj.y, obj.width, obj.height)
    });
}

void LevelGenerator::createPlatformObject(const GeneratedObject& obj)
{
    go->all.push_back(go->fabric->createPlatform(obj.x, obj.y, obj.width, obj.height));
}

void LevelGenerator::createGroundBlockObject(const GeneratedObject& obj)
{
    go->all.push_back(go->fabric->createGroundBlock(obj.x, obj.y, obj.width, obj.height));
}

void LevelGenerator::createTreeObject(const GeneratedObject& obj)
{
    Tree* tree = go->fabric->createTree(obj.x, obj.y, obj.trunkHeight, obj.numBranches,
                                        obj.minBranchLength, obj.maxBranchLength, obj.branchStep,
                                        obj.topBranchSide, obj.topBranchLength);
    // Add only branches to all (for collision)
    for (Platform* branch : tree->getBranches())
    {
        go->all.push_back(branch);
    }
    go->all.push_back(tree);
}

void LevelGenerator::createHeartObject(const GeneratedObject& obj)
{
    go->all.push_back(go->fabric->createHeart(obj.x, obj.y, obj.width, obj.height));
}

void LevelGenerator::createAmmoObject(const GeneratedObject& obj)
{
    go->all.push_back(go->fabric->createAmmo(obj.x, obj.y, obj.width, obj.height));
}

void LevelGenerator::createCrystalObject(const GeneratedObject& obj)
{
    go->all.push_back(go->fabric->createCrystal(obj.x, obj.y, obj.width, obj.height));
}

void LevelGenerator::createShooterObject(const GeneratedObject& obj)
{
    go->all.push_back(go->fabric->createShooter(obj.x, obj.y, obj.width, obj.height));
}

void LevelGenerator::createFlagObject(const GeneratedObject& obj)
{
    go->all.push_back(go->fabric->createFlag(obj.x, obj.y, obj.width, obj.height));
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
    // Spatial hash grid: only blocks in nearby cells are checked for clearance,
    // so the number of proximity checks stays linear in the number of blocks.
    grid = new SpacingGrid(SPACING_CELL_SIZE, static_cast<float>(HALF_GAP));

    GeneratedObject startBlock = makeStartBlock();
    grid->insert(spacingBoxOf(startBlock));
    objects.push_back(startBlock);

    std::vector<Node> frontier;
    frontier.push_back({startBlock, 0, objects.size() - 1});
    int groundCount = 1;

    growFrontier(frontier, objects, groundCount);

    delete grid;
    grid = nullptr;
}

GeneratedObject LevelGenerator::makeStartBlock()
{
    // Fixed start block in the bottom-left corner, on the ground.
    GeneratedObject startBlock;
    startBlock.x = START_BLOCK_X;
    startBlock.y = START_BLOCK_Y;
    startBlock.width = START_BLOCK_WIDTH;
    startBlock.height = START_BLOCK_HEIGHT;
    startBlock.type = "GroundBlock";
    return startBlock;
}

int LevelGenerator::generateBlockWidth()
{
    const int baseTiles = pickBaseTiles();
    const float scale = randomFloat(WIDTH_SCALE_MIN, WIDTH_SCALE_MAX);
    const int tiles = static_cast<int>(baseTiles * scale + 0.5f);
    return std::max(tiles, 3) * PLATFORM_TILE_WIDTH;
}

int LevelGenerator::pickBaseTiles()
{
    // Base length 3-10 tiles, more long than short.
    if (randomFloat(0, 1) < 0.6)
    {
        return randomInt(6, 10);
    }
    return randomInt(3, 5);
}

void LevelGenerator::growFrontier(std::vector<Node>& frontier,
                                  std::vector<GeneratedObject>& objects, int& groundCount)
{
    int guard = 0;

    while (!frontier.empty() && guard < FRONTIER_GUARD_LIMIT)
    {
        ++guard;
        std::vector<Node> next = nextFrontier(frontier, objects, groundCount);
        frontier.swap(next);
    }
}

std::vector<LevelGenerator::Node> LevelGenerator::nextFrontier(
    const std::vector<Node>& frontier, std::vector<GeneratedObject>& objects, int& groundCount)
{
    std::vector<Node> next;

    for (const Node& lower : frontier)
    {
        expandNode(lower, objects, next, groundCount);
    }

    return next;
}

void LevelGenerator::expandNode(const Node& lower, std::vector<GeneratedObject>& objects,
                                std::vector<Node>& next, int& groundCount)
{
    GeneratedObject candidate;
    int side = -1;

    if (!placeForLower(lower, candidate, side))
    {
        return;
    }

    registerNeighbour(lower, candidate, side, objects, next, groundCount);
}

void LevelGenerator::registerNeighbour(const Node& lower, const GeneratedObject& candidate,
                                       int side, std::vector<GeneratedObject>& objects,
                                       std::vector<Node>& next, int& groundCount)
{
    if (side >= 0)
    {
        objects[lower.objectIndex].topBranchSide = side;
    }

    objects.push_back(candidate);
    next.push_back({candidate, groundCount, objects.size() - 1});
    ++groundCount;
}

bool LevelGenerator::placeForLower(const Node& lower, GeneratedObject& out, int& side)
{
    const bool carriesTree = isTreeCandidate(lower.block, lower.index);
    if (!carriesTree)
    {
        side = -1;
        return placeUpward(lower, false, -1, out);
    }

    return placeTreeNeighbour(lower, out, side);
}

bool LevelGenerator::placeTreeNeighbour(const Node& lower, GeneratedObject& out, int& side)
{
    const int preferred = choosePreferredSide();

    if (placeUpward(lower, true, preferred, out))
    {
        side = preferred;
        return true;
    }

    return placeOtherSide(lower, out, side, preferred);
}

bool LevelGenerator::placeOtherSide(const Node& lower, GeneratedObject& out, int& side, int preferred)
{
    if (placeUpward(lower, true, 1 - preferred, out))
    {
        side = 1 - preferred;
        return true;
    }

    side = -1;
    return false;
}

int LevelGenerator::choosePreferredSide()
{
    return (randomFloat(0, 1) < 0.5f) ? 0 : 1;
}

bool LevelGenerator::placeUpward(const Node& lower, bool carriesTree, int forcedSide,
                                 GeneratedObject& out)
{
    if (!hasRoomAbove(lower))
    {
        return false;
    }

    const int y = upwardY(lower, carriesTree);
    return placeWithinHeight(lower, y, carriesTree, forcedSide, out);
}

bool LevelGenerator::hasRoomAbove(const Node& lower)
{
    return lower.block.y > MIN_PLATFORM_Y;
}

int LevelGenerator::upwardY(const Node& lower, bool carriesTree)
{
    // A tree-carrying block leaves a full tree height of room; the tree's
    // branches bridge the gap. Other blocks keep a jump-sized step.
    const int step = carriesTree ? TREE_HEIGHT : randomInt(50, 100);
    return static_cast<int>(lower.block.y) - step;
}

bool LevelGenerator::placeWithinHeight(const Node& lower, int y, bool carriesTree,
                                       int forcedSide, GeneratedObject& out)
{
    if (y < MIN_PLATFORM_Y)
    {
        return false;
    }

    return placeUpwardAttempts(lower, y, forcedSide, out);
}

bool LevelGenerator::placeUpwardAttempts(const Node& lower, int y, int forcedSide,
                                         GeneratedObject& out)
{
    bool placed = false;

    for (int attempt = 0; attempt < UPWARD_ATTEMPTS && !placed; ++attempt)
    {
        placed = tryCommitAttempt(lower, y, forcedSide, out);
    }

    return placed;
}

bool LevelGenerator::tryCommitAttempt(const Node& lower, int y, int forcedSide,
                                      GeneratedObject& out)
{
    GeneratedObject candidate = makeUpwardCandidate(lower, y, forcedSide);
    if (!canCommitCandidate(candidate))
    {
        return false;
    }

    grid->insert(spacingBoxOf(candidate));
    out = candidate;
    return true;
}

GeneratedObject LevelGenerator::makeUpwardCandidate(const Node& lower, int y, int forcedSide)
{
    const int width = generateBlockWidth();
    const int gap = randomInt(MIN_BLOCK_GAP, MAX_HORIZONTAL_REACH);
    const bool left = chooseSide(forcedSide);

    GeneratedObject candidate;
    candidate.x = candidateX(lower.block, left, gap, width);
    candidate.y = static_cast<float>(y);
    candidate.width = width;
    candidate.height = PLATFORM_BLOCK_HEIGHT;
    candidate.type = "GroundBlock";
    return candidate;
}

bool LevelGenerator::chooseSide(int forcedSide)
{
    if (forcedSide >= 0)
    {
        return forcedSide == 0;
    }

    return randomFloat(0, 1) < 0.5f;
}

float LevelGenerator::candidateX(const GeneratedObject& lower, bool left, int gap, int width)
{
    if (left)
    {
        return lower.x - static_cast<float>(gap) - width;
    }

    return lower.x + lower.width + static_cast<float>(gap);
}

bool LevelGenerator::canCommitCandidate(const GeneratedObject& candidate)
{
    return isWithinLevel(candidate) && grid->canPlace(spacingBoxOf(candidate));
}

bool LevelGenerator::isWithinLevel(const GeneratedObject& candidate)
{
    return candidate.x >= LEVEL_EDGE_MARGIN &&
           candidate.x + candidate.width <= LEVEL_WIDTH - LEVEL_EDGE_MARGIN;
}

bool LevelGenerator::isGroundBlock(const GeneratedObject& object)
{
    return object.type == "GroundBlock";
}

bool LevelGenerator::isNthPlatform(size_t index, int step)
{
    return (index + 1) % static_cast<size_t>(step) == 0;
}

bool LevelGenerator::platformHasTree(const std::vector<GeneratedObject>& objects,
                                     const GeneratedObject& block)
{
    return std::any_of(objects.begin(), objects.end(), TreeOnBlock{block});
}

std::vector<GeneratedObject> LevelGenerator::groundBlocks(
    const std::vector<GeneratedObject>& objects)
{
    std::vector<GeneratedObject> blocks;
    std::copy_if(objects.begin(), objects.end(), std::back_inserter(blocks), isGroundBlock);
    return blocks;
}

std::vector<size_t> LevelGenerator::groundBlockIndices(
    const std::vector<GeneratedObject>& objects)
{
    std::vector<size_t> indices;

    for (size_t i = 0; i < objects.size(); ++i)
    {
        addGroundBlockIndex(indices, objects[i], i);
    }

    return indices;
}

void LevelGenerator::addGroundBlockIndex(std::vector<size_t>& indices,
                                         const GeneratedObject& object, size_t index)
{
    if (isGroundBlock(object))
    {
        indices.push_back(index);
    }
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

void LevelGenerator::generateGuaranteedItem(std::vector<GeneratedObject>& objects, int itemFlag,
                                            ItemPlacer placer) const
{
    const std::vector<GeneratedObject> blocks = groundBlocks(objects);

    for (size_t i = 0; i < blocks.size(); ++i)
    {
        placeGuaranteedItem(objects, blocks[i], i, itemFlag, placer);
    }
}

void LevelGenerator::placeGuaranteedItem(std::vector<GeneratedObject>& objects,
                                         const GeneratedObject& block, size_t index,
                                         int itemFlag, ItemPlacer placer) const
{
    const int mask = effectiveItemMask(objects, block, index);
    if (!(mask & itemFlag))
    {
        return;
    }

    const int count = popcount3(mask);
    const int slot = slotForItem(mask, itemFlag);
    placer(objects, block, itemSlotX(block, slot, count));
}

void LevelGenerator::generateGuaranteedAmmo(std::vector<GeneratedObject>& objects) const
{
    generateGuaranteedItem(objects, ITEM_AMMO, &LevelGenerator::placeAmmunition);
}

void LevelGenerator::generateGuaranteedHeart(std::vector<GeneratedObject>& objects) const
{
    generateGuaranteedItem(objects, ITEM_HEART, &LevelGenerator::placeHeart);
}

void LevelGenerator::generateGuaranteedCrystal(std::vector<GeneratedObject>& objects) const
{
    generateGuaranteedItem(objects, ITEM_CRYSTAL, &LevelGenerator::placeCrystal);
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
        crystalOrdinal = placeEnemyForPlatform(objects, indices[k], k, crystalOrdinal);
    }
}

int LevelGenerator::placeEnemyForPlatform(std::vector<GeneratedObject>& objects,
                                          size_t platformIndex, size_t k,
                                          int crystalOrdinal) const
{
    const GeneratedObject& platform = objects[platformIndex];
    const bool hasCrystal = platformHasCrystal(objects, platform, k);
    const int ordinal = incrementIfCrystal(crystalOrdinal, hasCrystal);

    appendEnemyIfNeeded(objects, platform, hasCrystal, ordinal);
    return ordinal;
}

int LevelGenerator::incrementIfCrystal(int crystalOrdinal, bool hasCrystal)
{
    if (hasCrystal)
    {
        return crystalOrdinal + 1;
    }

    return crystalOrdinal;
}

void LevelGenerator::appendEnemyIfNeeded(std::vector<GeneratedObject>& objects,
                                         const GeneratedObject& platform, bool hasCrystal,
                                         int crystalOrdinal) const
{
    if (shouldPlaceEnemy(platform, hasCrystal, crystalOrdinal))
    {
        objects.push_back(makeEnemy(platform));
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

bool LevelGenerator::blocksOverhead(const GeneratedObject& candidate,
                                    const GeneratedObject& block, float trunkX,
                                    int trunkWidth)
{
    const bool above = candidate.y + candidate.height <= block.y;
    const bool overlapsX = trunkX < candidate.x + candidate.width &&
                           trunkX + trunkWidth > candidate.x;
    return above && overlapsX;
}

int LevelGenerator::minClearance(int worst, const GeneratedObject& candidate,
                                 const GeneratedObject& block, float trunkX, int trunkWidth)
{
    if (!blocksOverhead(candidate, block, trunkX, trunkWidth))
    {
        return worst;
    }

    const int clearance = static_cast<int>(block.y) -
                          (static_cast<int>(candidate.y) + candidate.height);
    return std::min(worst, clearance);
}

int LevelGenerator::clearanceAtX(const std::vector<GeneratedObject>& objects,
                                 const GeneratedObject& block, float trunkX,
                                 int trunkWidth)
{
    int worst = INT_MAX;

    for (const auto& candidate : objects)
    {
        worst = minClearance(worst, candidate, block, trunkX, trunkWidth);
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
        appendClearPosition(positions, objects, block, x);
    }

    return positions;
}

void LevelGenerator::appendClearPosition(std::vector<float>& positions,
                                         const std::vector<GeneratedObject>& objects,
                                         const GeneratedObject& block, float x)
{
    if (clearanceAtX(objects, block, x, Tree::TRUNK_WIDTH) >= TREE_HEIGHT)
    {
        positions.push_back(x);
    }
}

bool LevelGenerator::isTreeCandidate(const GeneratedObject& block, size_t index)
{
    return (index + 1) % static_cast<size_t>(TREE_PLATFORM_INTERVAL) == 0 &&
           block.width >= TREE_MIN_BLOCK_WIDTH;
}

bool LevelGenerator::buildTree(std::vector<GeneratedObject>& objects, size_t blockIndex)
{
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
    treeObj.minBranchLength = TREE_MIN_BRANCH_LENGTH;
    treeObj.maxBranchLength = TREE_MAX_BRANCH_LENGTH;
    treeObj.branchStep = TREE_BRANCH_STEP;
    treeObj.topBranchSide = block.topBranchSide;
    treeObj.topBranchLength = topBranchLengthFor(block, x, block.topBranchSide);
    objects.push_back(treeObj);
    return true;
}

float LevelGenerator::branchOuterEnd(const GeneratedObject& block, float trunkX, int side)
{
    if (side == 0)
    {
        return trunkX + Tree::TRUNK_WIDTH;
    }

    return block.x + block.width;
}

float LevelGenerator::branchInnerEnd(const GeneratedObject& block, float trunkX, int side)
{
    if (side == 0)
    {
        return block.x;
    }

    return trunkX;
}

int LevelGenerator::topBranchLengthFor(const GeneratedObject& block, float trunkX, int side)
{
    // The top branch base sits at the far trunk edge and reaches the host block
    // edge on the branch side, so the player can walk out to the edge and jump
    // across the reserved gap to the platform above.
    return static_cast<int>(branchOuterEnd(block, trunkX, side) -
                            branchInnerEnd(block, trunkX, side));
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
        buildTreeIfCandidate(objects, indices[k], k);
    }
}

void LevelGenerator::buildTreeIfCandidate(std::vector<GeneratedObject>& objects,
                                          size_t blockIndex, size_t k)
{
    if (isTreeCandidate(objects[blockIndex], k))
    {
        buildTree(objects, blockIndex);
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
