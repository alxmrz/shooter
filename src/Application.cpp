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
    window    = new Window();
    gameState = new GameState(this);
    event     = new Event(this, gameState);
    scene     = new Scene(this, gameState);
}

Application::~Application()
{
    delete window;
    delete gameState;
    delete event;
    delete scene;
}

int Application::run() {
    window->init();
    scene->initMainMenu();

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