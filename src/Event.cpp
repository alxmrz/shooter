#include <iostream>
#include "Event.h"
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
    
     while (this->app->window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed) 
            {
                this->app->window->close();
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
            {
                this->gameState->texts[2]->setString("Left mouse pressed");
                sf::Vector2i globalPosition = sf::Mouse::getPosition(*this->app->window);
                if (this->gameState->builder->collidePoint(globalPosition.x, globalPosition.y)) {
                    this->gameState->builder->setFormColor(sf::Color::Yellow);
                } else {
                    this->gameState->builder->setFormColor(sf::Color::Red);
                }
                this->gameState->texts[3]->setString(std::to_string(globalPosition.x) + "|" + std::to_string(globalPosition.y));
                this->gameState->shape.setFillColor(sf::Color::Red);
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                this->gameState->texts[2]->setString("Right mouse pressed");
                this->gameState->shape.setFillColor(sf::Color::Blue);
                this->gameState->builder->setFormColor(sf::Color::Blue);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                this->gameState->builder1->move(0, -10);
                this->gameState->view->move(0, -10);
                if (this->gameState->builder1->collideRect(this->gameState->builder)) {
                    this->gameState->builder1->move(0, 10);
                    this->gameState->view->move(0, 10);
                }
            } 
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                this->gameState->builder1->move(0, 10);
                this->gameState->view->move(0, 10);

                if (this->gameState->builder1->collideRect(this->gameState->builder)) {
                    this->gameState->builder1->move(0, -10);
                    this->gameState->view->move(0, -10);
                } 
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                this->gameState->builder1->move(-10, 0);
                this->gameState->view->move(-10.f, 0.f);
                if (this->gameState->builder1->collideRect(this->gameState->builder)) {
                    this->gameState->builder1->move(10, 0);
                    this->gameState->view->move(10, 0);
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                this->gameState->builder1->move(10, 0);
                this->gameState->view->move(10, 0);
                if (this->gameState->builder1->collideRect(this->gameState->builder)) {
                    this->gameState->builder1->move(-10, 0);
                    this->gameState->view->move(-10, 0);
                } 
            }  
            
            if(event.type == sf::Event::MouseWheelScrolled)
            {
                if(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                    if (event.mouseWheelScroll.delta == -1)
                    {
                        this->gameState->view->zoom(1.5);
                    }
                    else 
                    {
                       this->gameState->view->zoom(0.5); 
                    }
                    
                else if(event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
                    this->gameState->view->zoom(-1.f);
                else
                    std::cout << "wheel type: unknown" << std::endl;
                
                std::cout << "wheel movement: " << event.mouseWheelScroll.delta << std::endl;
            }
        }
}
