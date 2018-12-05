#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Application.h"

#include "Builder.h"
#include "Event.h"
#include "Scene.h"
#include "GameState.h"

Application::Application()
{
    this->gameState = new GameState(this);
    this->event = new Event(this, this->gameState);
    this->scene = new Scene(this, this->gameState);
}

Application::~Application()
{
    
}


int Application::run()
{
    this->initWindow();
    this->scene->init();
    
    while (this->window->isOpen())
    {
        this->event->handle();
        this->gameState->update();
        this->draw();
    }

    return 0;
}

void Application::draw()
{
    this->window->setView(*this->gameState->view);
    
    this->window->clear(sf::Color::White);
    for (unsigned i = 0; i < this->gameState->texts.size(); i++) {
        this->window->draw(*this->gameState->texts[i]);
    }

    this->window->draw(this->gameState->shape);
    this->window->draw(*this->gameState->builder->getDrawForm());
    this->window->draw(*this->gameState->builder1->getDrawForm());
    
    this->window->display();
}

void Application::initWindow()
{
    this->window = new sf::RenderWindow(sf::VideoMode(900, 600), "Robofarm");
    this->window->setFramerateLimit(60);
}