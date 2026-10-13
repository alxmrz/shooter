#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H

#include <map>
#include <vector>
#include <string>
#include <cstdlib>

class GameObjects;
class Map;
class SpacingGrid;

struct GeneratedObject {
    float x;
    float y;
    int width;
    int height;
    std::string type;
    
    // Tree-specific parameters
    int trunkHeight = 100;
    int numBranches = 3;
    int minBranchLength = 2;
    int maxBranchLength = 6;
    // If > 0 the tree grows evenly spaced "step" branches (a climbable ladder).
    int branchStep = 0;
    // Side of the topmost ladder branch: 0 = left, 1 = right. The reserved
    // platform above a tree is placed on the same side.
    int topBranchSide = 0;
    // Length in pixels the topmost ladder branch must span from the trunk to the
    // edge of the host ground block. 0 keeps the legacy random length.
    int topBranchLength = 0;
};

class LevelGenerator {
public:
    LevelGenerator(GameObjects* go);
    LevelGenerator(const LevelGenerator& orig);
    virtual ~LevelGenerator();
    
    void generateLevel(Map* map);
    
    // Public constants for testing
    static constexpr int LEVEL_WIDTH = 3000;
    static constexpr int LEVEL_HEIGHT = 3000;
    static constexpr int GROUND_Y = 2950;
    static constexpr int MAX_ENEMIES = 10;
    static constexpr int CAMERA_WIDTH = 800;
    static constexpr int CAMERA_HEIGHT = 600;

    // Player and jump geometry
    static constexpr int PLAYER_WIDTH = 50;
    static constexpr int PLAYER_HEIGHT = 50;
    static constexpr int GROUND_JUMP_HEIGHT = 100;
    static constexpr int AIR_JUMP_HEIGHT = 60;
    static constexpr int DOUBLE_JUMP_HEIGHT = GROUND_JUMP_HEIGHT + AIR_JUMP_HEIGHT;
    // Maximum allowed vertical distance between blocks: the player must be able
    // to reach the next block with a double jump, leaving room for its body.
    static constexpr int MAX_JUMP_HEIGHT = DOUBLE_JUMP_HEIGHT - PLAYER_HEIGHT;

    // Maximum horizontal gap between a ground block and its upward neighbour that
    // the player can still cross during a jump. Derived from the horizontal
    // movement physics: max horizontal speed is 5 pixels/frame at 60 FPS and a
    // double jump keeps the player airborne for roughly 37 frames, giving a
    // theoretical reach of about 185 pixels. A conservative 0.8 factor yields 150.
    static constexpr int MAX_HORIZONTAL_REACH = 150;

    // Fixed starting block in the bottom-left corner; the player spawns on it.
    static constexpr int START_BLOCK_X = 50;
    static constexpr int START_BLOCK_Y = GROUND_Y;
    static constexpr int START_BLOCK_WIDTH = 200;
    static constexpr int START_BLOCK_HEIGHT = 25;

    // A ground block must be at least this wide to carry a tree.
    static constexpr int TREE_MIN_BLOCK_WIDTH = 500;

    // Enemies: one per every second crystal-carrying ground block, placed
    // centered and kept outside the initial camera view.
    static constexpr int ENEMY_WIDTH = 50;
    static constexpr int ENEMY_HEIGHT = 50;
    static constexpr int ENEMY_CRYSTAL_STEP = 2;
    static constexpr int CAMERA_SAFE_X = CAMERA_WIDTH + 100;

    // Every generated tree is TREE_HEIGHT pixels tall from the trunk base to the
    // top of the leaves. Its branches are evenly spaced by TREE_BRANCH_STEP so
    // the player can climb the tree itself. A tree is only placed where a
    // TREE_HEIGHT-tall column above the trunk is clear of ground blocks; the
    // trunk position is chosen to fit, otherwise the block gets no tree.
    static constexpr int TREE_HEIGHT = 400;
    static constexpr int TREE_TRUNK_HEIGHT = 2 * TREE_HEIGHT / 3;
    static constexpr int TREE_BRANCH_STEP = 90;
    static constexpr int TREE_TRUNK_MARGIN = 100;
    static constexpr int TREE_COLUMN_STEP = 25;
    
    // Every third platform gets Ammunition, every fourth a Heart and every
    // second a Crystal; a platform carrying a tree is skipped.
    static constexpr int ITEM_SIZE = 50;
    static constexpr int AMMUNITION_STEP = 3;
    static constexpr int HEART_STEP = 4;
    static constexpr int CRYSTAL_STEP = 2;
    static constexpr int ITEM_AMMO = 1;
    static constexpr int ITEM_HEART = 2;
    static constexpr int ITEM_CRYSTAL = 4;

    // The flag marks the level goal and sits on the highest ground block.
    static constexpr int FLAG_SIZE = 50;

    // Public methods for testing
    void generateGround(std::vector<GeneratedObject>& objects);
    void generatePlatforms(std::vector<GeneratedObject>& objects);
    void generatePlatformItems(std::vector<GeneratedObject>& objects);
    void generateEnemies(std::vector<GeneratedObject>& objects);
    void generateTrees(std::vector<GeneratedObject>& objects);
    void generateFlag(std::vector<GeneratedObject>& objects);
    
    float randomFloat(float min, float max);
    int randomInt(int min, int max);
    
private:
    struct Node
    {
        GeneratedObject block;
        int index;
        size_t objectIndex;
    };

    typedef void (LevelGenerator::*ObjectCreator)(const GeneratedObject&);
    typedef std::map<std::string, ObjectCreator> CreatorTable;
    typedef void (*ItemPlacer)(std::vector<GeneratedObject>&, const GeneratedObject&, float);

    GameObjects* go;
    SpacingGrid* grid = nullptr;

    // Level assembly
    void createPlayer();
    void instantiateObjects(const std::vector<GeneratedObject>& objects);
    void instantiateObject(const GeneratedObject& obj);
    ObjectCreator creatorFor(const std::string& type);
    const CreatorTable& creatorTable();
    void createBorderObject(const GeneratedObject& obj);
    void createPlatformObject(const GeneratedObject& obj);
    void createGroundBlockObject(const GeneratedObject& obj);
    void createTreeObject(const GeneratedObject& obj);
    void createHeartObject(const GeneratedObject& obj);
    void createAmmoObject(const GeneratedObject& obj);
    void createCrystalObject(const GeneratedObject& obj);
    void createShooterObject(const GeneratedObject& obj);
    void createFlagObject(const GeneratedObject& obj);

    // Platform generation
    GeneratedObject makeStartBlock();
    int generateBlockWidth();
    int pickBaseTiles();
    void growFrontier(std::vector<Node>& frontier, std::vector<GeneratedObject>& objects,
                      int& groundCount);
    std::vector<Node> nextFrontier(const std::vector<Node>& frontier,
                                   std::vector<GeneratedObject>& objects, int& groundCount);
    void expandNode(const Node& lower, std::vector<GeneratedObject>& objects,
                    std::vector<Node>& next, int& groundCount);
    void registerNeighbour(const Node& lower, const GeneratedObject& candidate, int side,
                           std::vector<GeneratedObject>& objects, std::vector<Node>& next,
                           int& groundCount);
    bool placeForLower(const Node& lower, GeneratedObject& out, int& side);
    bool placeTreeNeighbour(const Node& lower, GeneratedObject& out, int& side);
    bool placeOtherSide(const Node& lower, GeneratedObject& out, int& side, int preferred);
    int choosePreferredSide();
    bool placeUpward(const Node& lower, bool carriesTree, int forcedSide, GeneratedObject& out);
    bool hasRoomAbove(const Node& lower);
    int upwardY(const Node& lower, bool carriesTree);
    bool placeWithinHeight(const Node& lower, int y, bool carriesTree, int forcedSide,
                           GeneratedObject& out);
    bool placeUpwardAttempts(const Node& lower, int y, int forcedSide, GeneratedObject& out);
    bool tryCommitAttempt(const Node& lower, int y, int forcedSide, GeneratedObject& out);
    GeneratedObject makeUpwardCandidate(const Node& lower, int y, int forcedSide);
    bool chooseSide(int forcedSide);
    float candidateX(const GeneratedObject& lower, bool left, int gap, int width);
    bool canCommitCandidate(const GeneratedObject& candidate);
    bool isWithinLevel(const GeneratedObject& candidate);

    // Shared generated-object queries
    static bool isGroundBlock(const GeneratedObject& object);
    static bool platformHasTree(const std::vector<GeneratedObject>& objects,
                                const GeneratedObject& block);
    static std::vector<size_t> groundBlockIndices(
        const std::vector<GeneratedObject>& objects);
    static void addGroundBlockIndex(std::vector<size_t>& indices,
                                    const GeneratedObject& object, size_t index);
    static std::vector<GeneratedObject> groundBlocks(
        const std::vector<GeneratedObject>& objects);
    static bool isNthPlatform(size_t index, int step);
    static bool isTreeCandidate(const GeneratedObject& block, size_t index);

    // Items
    static int itemMaskForIndex(size_t index);
    static int popcount3(int mask);
    static int slotForItem(int mask, int item);
    int effectiveItemMask(const std::vector<GeneratedObject>& objects,
                          const GeneratedObject& block, size_t index) const;
    static float itemSlotX(const GeneratedObject& block, int slot, int count);
    void generateGuaranteedItem(std::vector<GeneratedObject>& objects, int itemFlag,
                                ItemPlacer placer) const;
    void placeGuaranteedItem(std::vector<GeneratedObject>& objects,
                             const GeneratedObject& block, size_t index, int itemFlag,
                             ItemPlacer placer) const;
    static void placeAmmunition(std::vector<GeneratedObject>& objects,
                                const GeneratedObject& block, float x);
    static void placeHeart(std::vector<GeneratedObject>& objects,
                           const GeneratedObject& block, float x);
    static void placeCrystal(std::vector<GeneratedObject>& objects,
                             const GeneratedObject& block, float x);
    void generateGuaranteedAmmo(std::vector<GeneratedObject>& objects) const;
    void generateGuaranteedHeart(std::vector<GeneratedObject>& objects) const;
    void generateGuaranteedCrystal(std::vector<GeneratedObject>& objects) const;

    // Enemies
    bool platformHasCrystal(const std::vector<GeneratedObject>& objects,
                            const GeneratedObject& block, size_t index) const;
    static bool isEverySecondCrystalPlatform(int crystalOrdinal);
    static bool shouldPlaceEnemy(const GeneratedObject& platform, bool hasCrystal,
                                 int crystalOrdinal);
    static bool isEnemyOutsideCamera(const GeneratedObject& platform);
    static float enemySpawnX(const GeneratedObject& platform);
    static GeneratedObject makeEnemy(const GeneratedObject& platform);
    int placeEnemyForPlatform(std::vector<GeneratedObject>& objects, size_t platformIndex,
                              size_t k, int crystalOrdinal) const;
    static int incrementIfCrystal(int crystalOrdinal, bool hasCrystal);
    void appendEnemyIfNeeded(std::vector<GeneratedObject>& objects,
                             const GeneratedObject& platform, bool hasCrystal,
                             int crystalOrdinal) const;

    // Trees
    static bool blocksOverhead(const GeneratedObject& candidate,
                               const GeneratedObject& block, float trunkX,
                               int trunkWidth);
    static int clearanceAtX(const std::vector<GeneratedObject>& objects,
                            const GeneratedObject& block, float trunkX,
                            int trunkWidth);
    static int minClearance(int worst, const GeneratedObject& candidate,
                            const GeneratedObject& block, float trunkX, int trunkWidth);
    std::vector<float> clearTrunkPositions(
        const std::vector<GeneratedObject>& objects,
        const GeneratedObject& block);
    void appendClearPosition(std::vector<float>& positions,
                             const std::vector<GeneratedObject>& objects,
                             const GeneratedObject& block, float x);
    bool buildTree(std::vector<GeneratedObject>& objects, size_t blockIndex);
    void buildTreeIfCandidate(std::vector<GeneratedObject>& objects, size_t blockIndex, size_t k);
    static int topBranchLengthFor(const GeneratedObject& block, float trunkX, int side);
    static float branchOuterEnd(const GeneratedObject& block, float trunkX, int side);
    static float branchInnerEnd(const GeneratedObject& block, float trunkX, int side);

    // Flag
    static const GeneratedObject* highestGroundBlock(
        const std::vector<GeneratedObject>& objects);
    static void addGroundBlocks(std::vector<const GeneratedObject*>& out,
                                const std::vector<GeneratedObject>& objects);
    static void addIfGroundBlock(std::vector<const GeneratedObject*>& out,
                                 const GeneratedObject& object);
    static bool isAbove(const GeneratedObject* a, const GeneratedObject* b);
    static GeneratedObject makeFlag(const GeneratedObject& block);
};

#endif /* LEVELGENERATOR_H */
