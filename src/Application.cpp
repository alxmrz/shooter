#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Application.h"

#include "Builder.h"
#include "Event.h"
#include "Scene.h"
#include "GameState.h"
#include "Window.h"

Application::Application()
{
    gameState = new GameState(this);
    
    window = new Window(sf::VideoMode(900, 600), "Robofarm");
    
    event = new Event(this, gameState);
    scene = new Scene(this, gameState);
}

Application::~Application()
{
    
}

int Application::run()
{
    window->setFramerateLimit(fps);
    scene->init();
    
    while (window->isOpen())
    {
        event->handle();
        gameState->update();
        window->draw(gameState);
    }

    return 0;
}