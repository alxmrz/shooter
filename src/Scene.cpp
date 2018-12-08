#include <string>
#include <iostream>
#include "GameState.h"
#include "Application.h"
#include "Scene.h"
#include "Builder.h"
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
    
    /*this->gameState->gameObjects.push_back(this->gameState->makeText("", 100, 100));
    this->gameState->gameObjects.push_back(this->gameState->makeText("", 200, 100));
    this->gameState->gameObjects.push_back(this->gameState->makeText("", 300, 100));
    this->gameState->gameObjects.push_back(this->gameState->makeText("", 400, 200));*/
    //this->gameState->gameObjects.push_back(this->gameState->shape);
    //this->gameState->gameObjects.push_back(this->gameState->builder);
    //this->gameState->gameObjects.push_back(this->gameState->player);
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
    
    Builder* builder = new Builder(200,200, 100, 100);
    Builder* player = new Builder(500,200, 100, 100);
            
    gameState->objects.playable.push_back(builder);
    gameState->objects.playable.push_back(player);
    
    gameState->objects.player = player;
    
}