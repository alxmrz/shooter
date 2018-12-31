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
    doc.LoadFile("resources/levels/test.xml");
    const char* str = doc.FirstChildElement("map")
                        ->FirstChildElement("layer")
                        ->FirstChildElement("data")
                        ->GetText();
    tinyxml2::XMLElement* map = doc.FirstChildElement("map");
    
    generateBackground(str);
    generatePlayable(map);
}

void Map::generateBackground(std::string data)
{
    std::vector<std::string> lines;
    std::vector<std::string> lineChars;
    boost::trim(data);
    boost::split(lines, data, boost::algorithm::is_any_of("\n"), boost::token_compress_on);
    float x = 0;
    float y = 0;
     
    for (std::string line : lines) {
        x = 0;
        boost::split(lineChars, line, boost::algorithm::is_any_of(","), boost::token_compress_on);
        for (auto c : lineChars) {
            if (c == "9") {
                go->background.push_back({9, x, y});
            } 
            x += 50;
        }
        y += 50;
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