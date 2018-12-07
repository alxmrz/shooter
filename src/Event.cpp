#include <SFML/Graphics.hpp>
#include <iostream>
#include "Event.h"
#include "Application.h"
#include "GameState.h"

Event::Event() {
}

Event::Event(Application* app, GameState* gameState) {
    this->app = app;
    this->gameState = gameState;
}

Event::~Event() {
}

void Event::handle()
{
    sf::Event event;
    
     while (app->window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed) 
            {
                app->window->close();
            }
            
            handleKeys(&event);
            handelArrowKeys(&event);
            handleMouseKeys(&event);
            handleUiButtonsEvents(&event);

        }
}

void Event::handleKeys(sf::Event* event)
{
    
}

void Event::handelArrowKeys(sf::Event* event)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        gameState->player->move(0, -10);
        gameState->view->move(0, -10);
        if (gameState->player->collideRect(gameState->builder)) {
            gameState->player->move(0, 10);
            gameState->view->move(0, 10);
        }
    } 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        gameState->player->move(0, 10);
        gameState->view->move(0, 10);

        if (gameState->player->collideRect(gameState->builder)) {
            gameState->player->move(0, -10);
            gameState->view->move(0, -10);
        } 
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        gameState->player->move(-10, 0);
        gameState->view->move(-10.f, 0.f);
        if (gameState->player->collideRect(gameState->builder)) {
            gameState->player->move(10, 0);
            gameState->view->move(10, 0);
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        gameState->player->move(10, 0);
        gameState->view->move(10, 0);
        if (gameState->player->collideRect(gameState->builder)) {
            gameState->player->move(-10, 0);
            gameState->view->move(-10, 0);
        } 
    }  
}

void Event::handleMouseKeys(sf::Event* event)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
    {
        sf::Vector2i globalPosition = sf::Mouse::getPosition(*app->window);
        if (gameState->builder->collidePoint(globalPosition.x, globalPosition.y)) {
            gameState->builder->setFormColor(sf::Color::Yellow);
        } else {
            gameState->builder->setFormColor(sf::Color::Red);
        }
        
        //gameState->shape.setFillColor(sf::Color::Red);
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        
        //gameState->shape.setFillColor(sf::Color::Blue);
        gameState->builder->setFormColor(sf::Color::Blue);
    }
    
    if(event->type == sf::Event::MouseWheelScrolled)
    {
        if(event->mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            if (event->mouseWheelScroll.delta == -1)
            {
                gameState->view->zoom(1.5);
            }
            else 
            {
               gameState->view->zoom(0.5); 
            }

        else if(event->mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
            gameState->view->zoom(-1.f);
        else
            std::cout << "wheel type: unknown" << std::endl;

        std::cout << "wheel movement: " << event->mouseWheelScroll.delta << std::endl;
    }
}

void Event::handleUiButtonsEvents(sf::Event* event)
{
    
}