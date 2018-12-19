#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Application.h"
#include "GameState.h"
#include "Event.h"
#include "Scene.h"
#include "Window.h"

Application::Application()
{
    gameState = new GameState(this);

    window = new Window(sf::VideoMode(900, 600), "Shooter");

    event = new Event(this, gameState);
    scene = new Scene(this, gameState);
}

Application::~Application()
{
}

int Application::run()
{
    window->setFramerateLimit(fps);
    scene->initMainMenu();
    /*gameState->startNewGame*/

    while (window->isOpen()) {
        event->handle();
        gameState->update();
        window->draw(gameState);
    }

    return 0;
}

void Application::close()
{
    if (gameState->isGameStarted) {
        gameState->stopBackgroundMusic();
    }
    window->close();
}