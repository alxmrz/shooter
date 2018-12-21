#include <string>
#include <vector>
#include <iostream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "../include/tinyxml/tinyxml2.h"
#include "GameState.h"
#include "Application.h"
#include "Scene.h"
#include "objects/Ground.h"
#include "objects/Crystal.h"
#include "objects/Shooter.h"
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
    generateLevelNew();
}

void Scene::generateLevel()
{
    std::string level =
            "                                     S       |"
            "               C    S         C   GGGGG      |"
            "   S        GGGGGGGGGGGG                     |"
            "GGGGGGG                      GGGGG           |"
            "       C                   S                 |"
            "    GGGGGG               GGGGG               |"
            " GGG          C                            GG|"
            " GG                 GGG                    G|"
            " GGGGGGG                            C       G|"
            " GG   C       GGGG      C      C    C    C  G|"
            " GGP       S     S     S       S        S   G|"
            "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG|"
            ;
    std::vector<std::string> lines;
    boost::split(lines, level, boost::algorithm::is_any_of("|"), boost::token_compress_on);
    float x = 0;
    float y = 0;
    Ground* g;
    for (std::string line : lines) {
        x = 0;
        for (char c : line) {
            if (c == 'G') {
                g = gameState->objects->fabric->createGround(x, y, 50, 50);            
                gameState->objects->background.push_back(g);
                gameState->objects->backgrounds[x][y] = g;
            } else if (c == 'P') {
                Shooter* shooter = gameState->objects->fabric->createShooter(x, y, 50, 50);
                shooter->setMain(true);
                gameState->objects->player = shooter;
                gameState->objects->playable.push_back(shooter);
            } else if (c == 'S') {
                Shooter* shooter = gameState->objects->fabric->createShooter(x, y, 50, 50);
                gameState->objects->playable.push_back(shooter);
            } else if (c == 'C') {
                gameState->objects->crystals.push_back(
                    gameState->objects->fabric->createCrystal(x, y, 50, 50)
                );
            }
            x += 50;
        }
        y += 50;
    }
}

void Scene::generateLevelNew()
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile("resources/levels/test.xml");
    const char* str = doc.FirstChildElement("map")
                        ->FirstChildElement("layer")
                        ->FirstChildElement("data")
                        ->GetText();
    tinyxml2::XMLElement* objectgroup = doc.FirstChildElement("map")->FirstChildElement("objectgroup");
    
    generateBackground(str);
    generatePlayable(objectgroup);
}

void Scene::generateBackground(std::string data)
{
    std::vector<std::string> lines;
    std::vector<std::string> lineChars;
    
    boost::split(lines, data, boost::algorithm::is_any_of("\n"), boost::token_compress_on);
    float x = 0;
    float y = 0;
    //Ground* g;
    for (std::string line : lines) {
        x = 0;
        boost::split(lineChars, line, boost::algorithm::is_any_of(","), boost::token_compress_on);
        for (auto c : lineChars) {
            if (c == "9") {
                Ground* g = gameState->objects->fabric->createGround(x, y, 50, 50);            
                gameState->objects->background.push_back(g);
                
                if (gameState->objects->backgrounds[x][y] == nullptr) {
                    gameState->objects->backgrounds[x][y] = g;
                }
            } 
            x += 50;
        }
        y += 50;
    }
}

void Scene::generatePlayable(tinyxml2::XMLElement* objectgroup)
{
    std::string type;
    tinyxml2::XMLElement* object;
    for (tinyxml2::XMLNode* node = objectgroup->FirstChildElement("object"); 
        node; 
        node = node->NextSibling()
        ) {
        object = node->ToElement();

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
        }
    } 
}