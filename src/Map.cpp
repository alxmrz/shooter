#include <string>
#include <iostream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "../include/tinyxml/tinyxml2.h"
#include "Map.h"
#include "GameObjects.h"
#include "objects/backgrounds/Ground.h"
#include "objects/interactive/Crystal.h"
#include "objects/interactive/Heart.h"
#include "objects/interactive/Ammunition.h"
#include "objects/interactive/Platform.h"
#include "objects/units/Shooter.h"
#include "Fabric.h"

Map::Map() 
{
}

Map::Map(const Map& orig) 
{
}

Map::Map(GameObjects* go) :go(go)
{
}

Map::~Map() 
{
    
}

void Map::generateLevel(const char* level)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(level);

    tinyxml2::XMLElement* map = doc.FirstChildElement("map");
    generateTIleIdPositions();
    generateBackground(map);
    generatePlayable(map);
}

void Map::generateBackground(tinyxml2::XMLElement* map)
{
    for (tinyxml2::XMLElement* layer = map->FirstChildElement("layer");
            layer != NULL;
            layer = layer->NextSiblingElement()) {
        
        if (layer->FirstChildElement("data") == NULL) break;
        std::string data = layer->FirstChildElement("data")->GetText();
        
        std::vector<std::string> lines;
        std::vector<std::string> lineChars;
        boost::trim(data);
        boost::split(lines, data, boost::algorithm::is_any_of("\n"), boost::token_compress_on);
        int x = 0;
        int y = 0;

        for (std::string line : lines) {
            x = 0;
            boost::split(lineChars, line, boost::algorithm::is_any_of(","), boost::token_compress_on);
            for (auto c : lineChars) {
                unsigned id = atoi(c.c_str()) - 1;
                if (id > 0 && id < go->globalTileIds.size()) {
                    go->background.push_back({x, y, go->globalTileIds[id][0], go->globalTileIds[id][1]});
                }

                x += 25;
            }
            y += 25;
        }
    }
}

void Map::generatePlayable(tinyxml2::XMLElement* map)
{
    std::string type;
    for (tinyxml2::XMLElement* objectgroup = map->FirstChildElement("objectgroup");
            objectgroup != NULL;
            objectgroup = objectgroup->NextSiblingElement()) {
        
        for (tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");
                object;
                object = object->NextSiblingElement()
                ) {
            if (object->Attribute("type") == 0) {
                continue;
            }
            
            type = object->Attribute("type");
            
            /**
             * TODO: fix DRY
             */
            if (type == "Shooter") {
                Shooter* shooter = go->fabric->createShooter(atoi(object->Attribute("x")), atoi(object->Attribute("y")), 50, 50);
                std::string name = object->Attribute("name");
                if (name == "Player") {
                    shooter->setMain(true);
                    go->player = shooter;
                }

                go->all.push_back(shooter);
            } else if (type == "Crystal") {
                go->all.push_back(
                        go->fabric->createCrystal(atoi(object->Attribute("x")), atoi(object->Attribute("y")), 50, 50)
                        );
            } else if (type == "Border") {
                go->borders.insert({atoi(object->Attribute("x")),
                    go->fabric->createBorder(
                        atoi(object->Attribute("x")), 
                        atoi(object->Attribute("y")), 
                        atoi(object->Attribute("width")), 
                        atoi(object->Attribute("height"))
                        )}
                );
            } else if (type == "Heart") {
                go->all.push_back(
                    go->fabric->createHeart(
                        atoi(object->Attribute("x")),
                        atoi(object->Attribute("y")),
                        50,
                        50
                    )
                ); 
            } else if (type == "Ammunition") {
                go->all.push_back(
                    go->fabric->createAmmo(
                        atoi(object->Attribute("x")),
                        atoi(object->Attribute("y")),
                        50,
                        50
                    )
                ); 
            } else if (type == "Platform") {
                go->all.push_back(
                    go->fabric->createPlatform(
                        atoi(object->Attribute("x")),
                        atoi(object->Attribute("y")),
                        100,
                        25
                    )
                ); 
            }
        }
    }
}

void Map::generateTIleIdPositions()
{
    /**
     * TODO: need to remove strict values from level template files
     * Current values are stored in files: training.tmx and *.tsx
     */
    int width = 1972;
    int height = 1184;
    int columns = 78;
    int margin = 0;
    int tileWidth = 25;
    int tileHeight = 25;
    int tilecount = 3542;
    
    int currentTileCount = 0;
    for (int y = margin; y < height - margin; y+=tileHeight) {
        for (int x = margin, column = 1; x < width-margin && column <= columns; x+=tileWidth, column++) {
            go->globalTileIds.push_back({x, y});
            currentTileCount++;
        }
    }
}