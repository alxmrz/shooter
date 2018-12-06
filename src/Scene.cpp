
#include "GameState.h"
#include "Application.h"
#include "Scene.h"
#include "ui/Button.h"

Scene::Scene() {
}

Scene::Scene(Application* app, GameState* gameState) {
    this->app = app;
    this->gameState = gameState;
}

Scene::~Scene() {
}

void Scene::init()
{
    
    this->gameState->view = new sf::View(sf::FloatRect(0.f, 0.f, 900.f, 600.f));
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
    this->gameState->gameObjects.push_back(this->gameState->builder);
    this->gameState->gameObjects.push_back(this->gameState->player);
    this->gameState->gameObjects.push_back(new Button(100, 100, 100, 100));
}
