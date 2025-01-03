#include <SFML/Graphics.hpp>
#include "Application.h"
#include "GameState.h"
#include "EventHandler.h"
#include "Scene.h"
#include "Window.h"

Application::Application()
{
    window    = new Window();
    gameState = new GameState(this);
    event     = new EventHandler(this, gameState);
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
        Event eventSubmitted = event->handle();
        gameState->update(eventSubmitted);
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