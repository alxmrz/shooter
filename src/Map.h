#ifndef MAP_H
#define MAP_H

#include <string>

namespace tinyxml2 {
    class XMLElement;
}

class GameObjects;

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
private:
    GameObjects* go;
    
    /**
     * Generate background objects positions.
     * Method only stores where must be places a special part of the global tileset
     * In first drawing background will be places into an image
     *  
     * @param map Xml node for map elements
     */
    void generateBackground(tinyxml2::XMLElement* map);
    
    /**
     * Generate game objects, which can be used for interactions
     * 
     * @param objectgroup
     */
    void generatePlayable(tinyxml2::XMLElement* objectgroup);
    
    /**
     * Generate parts of the global tileset, which will be used for background generation
     */
    void generateTIleIdPositions();
};

#endif /* MAP_H */

