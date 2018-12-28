#include <string>
#include <vector>
#include <iostream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "../include/tinyxml/tinyxml2.h"
#include "GameState.h"
#include "Application.h"
#include "Scene.h"
#include "objects/backgrounds/Ground.h"
#include "objects/interactive/Crystal.h"
#include "objects/interactive/Heart.h"
#include "objects/interactive/Ammunition.h"
#include "objects/units/Shooter.h"
#include "ui/Button.h"
#include "Fabric.h"

Scene::Scene()
{
}

Scene::Scene(Application* app, GameState* gameState)
{
    this->app = app;
    this->gameState = gameState;
    this->gameState->view = new sf::View(sf::FloatRect(0.f, 0.f, 900.f, 600.f));
}

Scene::~Scene()
{
}

void Scene::initMainMenu()
{
    this->gameState->objects->buttons.push_back(
        gameState->objects->fabric->createButton("start", "Start", 100, 100, 100, 30)
            
    );

    this->gameState->objects->buttons.push_back(
        gameState->objects->fabric->createButton("exit", "Exit", 100, 150, 100, 30)
    );
}

void Scene::initNewGame()
{
    gameState->objects->reset();
    generateLevel();
}

void Scene::generateLevel()
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

void Scene::generateBackground(std::string data)
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
                gameState->objects->background.push_back(
                        gameState->objects->fabric->createGround(x, y, 50, 50)
                        );
            } 
            x += 50;
        }
        y += 50;
    }
}

void Scene::generatePlayable(tinyxml2::XMLElement* map)
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
                Shooter* shooter = gameState->objects->fabric->createShooter(atoi(object->Attribute("x")), atoi(object->Attribute("y")), 50, 50);
                std::string name = object->Attribute("name");
                if (name == "Player") {
                    shooter->setMain(true);
                    gameState->objects->player = shooter;
                }

                gameState->objects->playable.push_back(shooter);
            } else if (type == "Crystal") {
                gameState->objects->crystals.push_back(
                        gameState->objects->fabric->createCrystal(atoi(object->Attribute("x")), atoi(object->Attribute("y")), 50, 50)
                        );
            } else if (type == "Border") {
                gameState->objects->borders.insert({atoi(object->Attribute("x")),
                    gameState->objects->fabric->createBorder(
                        atoi(object->Attribute("x")), 
                        atoi(object->Attribute("y")), 
                        atoi(object->Attribute("width")), 
                        atoi(object->Attribute("height"))
                        )}
                );
            } else if (type == "Heart") {
                gameState->objects->hearts.push_back(
                    gameState->objects->fabric->createHeart(
                        atoi(object->Attribute("x")),
                        atoi(object->Attribute("y")),
                        50,
                        50
                    )
                ); 
            } else if (type == "Ammunition") {
                gameState->objects->ammo.push_back(
                    gameState->objects->fabric->createAmmo(
                        atoi(object->Attribute("x")),
                        atoi(object->Attribute("y")),
                        50,
                        50
                    )
                ); 
            }
        }
    }
}