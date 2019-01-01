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

void Map::generateLevel()
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile("resources/levels/training.tmx");

    tinyxml2::XMLElement* map = doc.FirstChildElement("map");
    generateTIleIdPositions("basd");
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
                std::cout << "ID:" << id << std::endl;
                if (id > 0 && id < go->globalTileIds.size()) {
                    go->background.push_back({x, y, go->globalTileIds[id][0], go->globalTileIds[id][1]});
                }


                x += 25;
            }
            y += 25;
        }
    }
    
    

    
    std::cout << "FINISHED!" << std::endl;
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
            type = object->Attribute("type");

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

void Map::generateTIleIdPositions(std::string tileset)
{
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
    
    std::cout << "CURRENT TILE COUNT" << currentTileCount << std::endl;
    std::cout << "Tile 77 X:" << go->globalTileIds[77][0] << " Tile 77 Y:" << go->globalTileIds[77][1] << std::endl;
}