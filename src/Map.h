#ifndef MAP_H
#define MAP_H

#include <map>
#include <string>
#include <vector>

namespace tinyxml2 {
    class XMLElement;
}

class GameObjects;
class LevelGenerator;
class Shooter;
class Tree;

class Map {
public:
    Map();
    Map(GameObjects* go);
    Map(const Map& orig);
    virtual ~Map();
    /**
     * Generate level from string. Level string is inside the method until
     * count of levels is one
     */
    void generateLevel(const char* level);
    
    /**
     * Generate level programmatically using LevelGenerator
     */
    void generateLevel(LevelGenerator& generator);
private:
    typedef void (Map::*ObjectSpawner)(tinyxml2::XMLElement*);
    typedef std::map<std::string, ObjectSpawner> SpawnerTable;

    GameObjects* go;

    /**
     * Generate background objects positions.
     * Method only stores where must be places a special part of the global tileset
     * In first drawing background will be places into an image
     *  
     * @param map Xml node for map elements
     */
    void generateBackground(tinyxml2::XMLElement* map);
    void appendLayer(tinyxml2::XMLElement* layer);
    bool hasLayerData(tinyxml2::XMLElement* layer);
    std::string layerDataText(tinyxml2::XMLElement* layer);
    void appendLayerTiles(const std::string& data);
    std::vector<std::string> splitLayerLines(const std::string& data);
    void appendTileLine(const std::string& line, int y);
    void appendTile(const std::string& cell, int x, int y);
    
    /**
     * Generate game objects, which can be used for interactions
     * 
     * @param objectgroup
     */
    void generatePlayable(tinyxml2::XMLElement* objectgroup);
    void spawnGroupObjects(tinyxml2::XMLElement* objectgroup);
    void spawnObject(tinyxml2::XMLElement* object);
    bool hasType(tinyxml2::XMLElement* object);
    void runSpawner(tinyxml2::XMLElement* object, const std::string& type);
    ObjectSpawner spawnerFor(const std::string& type);
    const SpawnerTable& spawnerTable();
    int attrInt(tinyxml2::XMLElement* object, const char* name);

    void spawnShooter(tinyxml2::XMLElement* object);
    void setupPlayerIfNamed(tinyxml2::XMLElement* object, Shooter* shooter);
    bool isPlayerNamed(tinyxml2::XMLElement* object);
    void makeMainPlayer(tinyxml2::XMLElement* object, Shooter* shooter);
    void spawnCrystal(tinyxml2::XMLElement* object);
    void spawnBorder(tinyxml2::XMLElement* object);
    void spawnHeart(tinyxml2::XMLElement* object);
    void spawnAmmunition(tinyxml2::XMLElement* object);
    void spawnTree(tinyxml2::XMLElement* object);
    Tree* createTreeFromAttributes(tinyxml2::XMLElement* object);
    void appendTreePlatform(tinyxml2::XMLElement* object);
    void spawnGroundBlock(tinyxml2::XMLElement* object);
    
    /**
     * Generate parts of the global tileset, which will be used for background generation
     */
    void generateTIleIdPositions();
    void appendTileRow(int y);
};

#endif /* MAP_H */
