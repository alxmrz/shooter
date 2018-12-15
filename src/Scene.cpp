#include <string>
#include <vector>
#include <iostream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "GameState.h"
#include "Application.h"
#include "Scene.h"
#include "objects/Ground.h"
#include "objects/Shooter.h"
#include "ui/Button.h"

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
        new Button("start", "Start", 100, 100, 100, 30)
    );

    this->gameState->objects->buttons.push_back(
        new Button("exit", "Exit", 100, 150, 100, 30)
    );
}

void Scene::initNewGame()
{
    gameState->objects->reset();
    generateLevel();
}

void Scene::generateLevel()
{
    std::string level =
            "                                             |"
            "                                  GGGGG      |"
            "                                             |"
            "                             GGGGG           |"
            "                                             |"
            "                         GGGGG               |"
            " GGG                                       GG|"
            " GG                 GGGGG                   G|"
            " GG                                         G|"
            " GG           GGGGG                         G|"
            " GGP       S     S     S       S        S   G|"
            "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG|"
            ;
    std::vector<std::string> lines;
    boost::split(lines, level, boost::algorithm::is_any_of("|"), boost::token_compress_on);
    float x = 0;
    float y = 0;
    for (std::string line : lines) {
        x = 0;
        for (char c : line) {
            if (c == 'G') {
                gameState->objects->background.push_back(new Ground(x, y, 50, 50));
            } else if (c == 'P') {
                Shooter* shooter = new Shooter(gameState->objects, x, y, 50, 50);
                gameState->objects->player = shooter;
                gameState->objects->playable.push_back(shooter);
            } else if (c == 'S') {
                Shooter* shooter = new Shooter(gameState->objects, x, y, 50, 50);
                gameState->objects->playable.push_back(shooter);
            }
            x += 50;
        }
        y += 50;
    }
}