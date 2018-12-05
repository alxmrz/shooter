#include <iostream>
#include <string>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Application.h"
#include "Builder.h"

GameState::GameState() {
}

GameState::GameState(Application* app) {
    this->app = app;
    
}

GameState::~GameState() {
}


sf::Text* GameState::makeText(std::string str, int x, int y)
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
        text->setFillColor(sf::Color::Black);
        text->setPosition(x , y );
        text->setStyle(sf::Text::Bold | sf::Text::Underlined);
        return text; 
    }
}

void GameState::update()
{
        sf::Time elapsed = this->clock.getElapsedTime();
        
        /*if (this->right) {
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
            
        }*/
       
        float seconds = elapsed.asSeconds();
        if (seconds >= 1) {
           this->clock.restart();
           this->frame = 0;
        } else {
           this->frame++; 
        }
        
        /*this->texts[0]->setString(std::to_string(frame));
        this->texts[1]->setString(std::to_string(seconds));*/
}