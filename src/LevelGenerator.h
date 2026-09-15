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
    static constexpr int MAX_JUMP_HEIGHT = 110;
    
    // Public methods for testing
    void generateGround(std::vector<GeneratedObject>& objects);
    void generatePlatforms(std::vector<GeneratedObject>& objects);
    void generateItems(std::vector<GeneratedObject>& objects, const std::vector<GeneratedObject>& platforms);
    void generateEnemies(std::vector<GeneratedObject>& objects);
    
    float randomFloat(float min, float max);
    int randomInt(int min, int max);
    
private:
    GameObjects* go;
};

#endif /* LEVELGENERATOR_H */