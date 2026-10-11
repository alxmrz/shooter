#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H

#include <vector>
#include <cstdlib>
#include <string>

class GameObjects;
class Map;

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
    
    // Public methods for testing
    void generateGround(std::vector<GeneratedObject>& objects);
    void generatePlatforms(std::vector<GeneratedObject>& objects);
    void generateItems(std::vector<GeneratedObject>& objects, const std::vector<GeneratedObject>& platforms);
    void generateEnemies(std::vector<GeneratedObject>& objects);
    void generateTrees(std::vector<GeneratedObject>& objects);
    
    float randomFloat(float min, float max);
    int randomInt(int min, int max);
    
private:
    GameObjects* go;
    int findPlatformAtX(const std::vector<GeneratedObject>& platforms, float x);
};

#endif /* LEVELGENERATOR_H */