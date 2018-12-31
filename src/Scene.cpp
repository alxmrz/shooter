#include <string>
#include <vector>
#include <iostream>
#include "GameState.h"
#include "GameObjects.h"
#include "Map.h"
#include "Application.h"
#include "Scene.h"
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
    map = new Map(gameState->objects);
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
    map->generateLevel();
}