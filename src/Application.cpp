#include "Application.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <unistd.h>
#include "Builder.h"

Application::Application()
{
  
}

Application::~Application()
{
    
}

sf::Text* Application::makeText(std::string str, int x, int y)
{
    sf::Font* font = new sf::Font;
    if(!font->loadFromFile("fonts/arial.ttf")) {
        std::cout << "No fonts";
        throw;
    } else {
        sf::Text* text = new sf::Text;
        text->setFont(*font); 
        text->setString(str);
        text->setCharacterSize(12); 
        text->setFillColor(sf::Color::White);
        text->setPosition(x , y );
        text->setStyle(sf::Text::Bold | sf::Text::Underlined);
        return text; 
    }
    
}

int Application::run()
{
    this->initScene();
    
    while (this->window->isOpen())
    {
        this->handleEvents();
        this->updateGame();
        this->drawGameObjects();
    }

    return 0;
}

void Application::initScene()
{
    this->window = new sf::RenderWindow(sf::VideoMode(900, 600), "Infinity8loop");
    this->window->setFramerateLimit(60);
    
    this->shape = sf::CircleShape(20);
    this->builder = new Builder(200,200, 100, 100);
    this->shape.setFillColor(sf::Color::Green);
    this->clock = sf::Clock();
    
    this->texts.push_back(makeText("", 100, 100));
    this->texts.push_back(makeText("", 200, 100));
    this->texts.push_back(makeText("", 300, 100));
    this->texts.push_back(makeText("", 400, 200));
}

void Application::handleEvents()
{
    sf::Event event;
    
     while (this->window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed) 
            {
                this->window->close();
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
            {
                this->texts[2]->setString("Left mouse pressed");
                sf::Vector2i globalPosition = sf::Mouse::getPosition(*this->window);
                if (this->builder->collidePoint(globalPosition.x, globalPosition.y)) {
                    this->builder->setFormColor(sf::Color::Yellow);
                } else {
                    this->builder->setFormColor(sf::Color::Red);
                }
                this->texts[3]->setString(std::to_string(globalPosition.x) + "|" + std::to_string(globalPosition.y));
                this->shape.setFillColor(sf::Color::Red);
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                this->texts[2]->setString("Right mouse pressed");
                this->shape.setFillColor(sf::Color::Blue);
                this->builder->setFormColor(sf::Color::Blue);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                this->shape.move(0, 10);
            }
        }
}

void Application::updateGame()
{
        sf::Time elapsed = clock.getElapsedTime();
        
        if (this->right) {
            if (this->shape.getPosition().x >= 900) {
                this->right = false;
                this->left = true;
            } else {
                this->shape.move(3, 0);  
            }
        } else if(this->left) {
            if (this->shape.getPosition().x <= 0) {
                this->left = false;
                this->right = true;
            } else {
                this->shape.move(-3, 0);
            }
            
        }
       
        float seconds = elapsed.asSeconds();
        if (seconds >= 1) {
           this->clock.restart();
           this->frame = 0;
        } else {
           this->frame++; 
        }
        
        this->texts[0]->setString(std::to_string(frame));
        this->texts[1]->setString(std::to_string(seconds));
}

void Application::drawGameObjects()
{
    this->window->clear(sf::Color::White);
    for (unsigned i = 0; i < this->texts.size(); i++) {
        this->window->draw(*this->texts[i]);
    }

    this->window->draw(this->shape);
    this->window->draw(*this->builder->getDrawForm());
    this->window->display();
}