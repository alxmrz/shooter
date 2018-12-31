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
    void generateLevel();
private:
    GameObjects* go;
    
    void generateBackground(std::string data);
    void generatePlayable(tinyxml2::XMLElement* objectgroup);
};

#endif /* MAP_H */

