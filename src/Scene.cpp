#include <string>
#include <vector>
#include <iostream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "GameState.h"
#include "Application.h"
#include "Scene.h"
#include "Builder.h"
#include "objects/Ground.h"
#include "objects/Shooter.h"
#include "ui/Button.h"

Scene::Scene() {
}

Scene::Scene(Application* app, GameState* gameState) {
    this->app = app;
    this->gameState = gameState;
    this->gameState->view = new sf::View(sf::FloatRect(0.f, 0.f, 900.f, 600.f));
}

Scene::~Scene() {
}

void Scene::init()
{
    
    
    this->gameState->shape = sf::CircleShape(20);
    this->gameState->builder = new Builder(200,200, 100, 100);
    this->gameState->player = new Builder(500,200, 100, 100);
    //this->gameState->view->setCenter(500.f+50, 200.f+50);
    
    this->gameState->shape.setFillColor(sf::Color::Green);
    this->gameState->clock = sf::Clock();

    std::string str = "This is good";
    std::cout << "Before " << gameState->objects.buttons.size() << std::endl;
    this->gameState->objects.buttons.push_back(new Button("start", "Start", 100, 100, 100, 30));
    
    std::cout << "After " << gameState->objects.buttons.size() << std::endl;
}

void Scene::initMainMenu()
{
    this->gameState->objects.buttons.push_back(
            new Button("start", "Start", 100, 100, 100, 30)
    );
    
    this->gameState->objects.buttons.push_back(
            new Button("exit", "Exit", 100, 150, 100, 30)
    );
}
void Scene::initNewGame()
{
    gameState->objects.reset();
    generateLevel();
}

void Scene::generateLevel()
{
    std::string level = 
            "GGGGGGGGGGGGGGGGGG|"
            "G                G|"
            "G                G|"
            "GS               G|"
            "GGGGG            G|"
            "G    G         S G|"
            "G  P          SGGG|"
            "G GG         G   G|"
            "G GG        S    G|"
            "G GG      GGG    G|"
            "G GG           S G|"
            "GGGGGGGGGGGGGGGGGG|"
            ;
    std::vector<std::string> lines;
    boost::split(lines, level, boost::algorithm::is_any_of("|"), boost::token_compress_on);
    int x = 0;
    int y = 0;
    for (std::string line: lines) 
    {
        x=0;
        for(char c : line) 
        {
            if (c == 'G') 
            {
                gameState->objects.background.push_back(new Ground(x, y, 50, 50));
            }
            else if (c == 'P')
            {
                Shooter* shooter = new Shooter(&gameState->objects, x, y, 50, 50);
                gameState->objects.player = shooter;
                gameState->objects.playable.push_back(shooter);
            }
            else if (c == 'S')
            {
                Shooter* shooter = new Shooter(&gameState->objects, x, y, 50, 50);
                gameState->objects.playable.push_back(shooter);
            }
            x += 50;
        }
        y+=50;
    }
 
}