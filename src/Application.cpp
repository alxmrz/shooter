#include "Application.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <unistd.h>

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
    } else {
       sf::Text* text = new sf::Text;

        // select the font
        text->setFont(*font); // font is a sf::Font

        // set the string to display
        text->setString(str);

        // set the character size
        text->setCharacterSize(12); // in pixels, not points!

        // set the color
        text->setFillColor(sf::Color::White);
        text->setPosition(x , y );//задаем позицию текста, центр камеры

        // set the text style
        text->setStyle(sf::Text::Bold | sf::Text::Underlined);
        return text; 
    }
    
}

int Application::run()
{
    sf::RenderWindow window(sf::VideoMode(900, 600), "Infinity8loop");
    sf::CircleShape shape(20);
    shape.setFillColor(sf::Color::Green);
    sf::Clock clock;
    sf::Text* text = makeText("", 100, 100);
    sf::Text* time = makeText("", 200, 100);
    sf::Text* additional = makeText("", 300, 100);
    sf::Text* mousePosition = makeText("", 400, 200);
    int frame = 0;
    window.setFramerateLimit(60);
    bool right = true;
    bool left = false;
    while (window.isOpen())
    {
        sf::Event event;
        sf::Time elapsed = clock.getElapsedTime();
        
        
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
            {
                additional->setString("Left mouse pressed");
                sf::Vector2i globalPosition = sf::Mouse::getPosition(window);
                mousePosition->setString(std::to_string(globalPosition.x) + "|" + std::to_string(globalPosition.y));
                shape.setFillColor(sf::Color::Red);
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                additional->setString("Right mouse pressed");
                shape.setFillColor(sf::Color::Blue);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                shape.move(0, 10);
            }
        }
        if (right) {
            if (shape.getPosition().x >= 900) {
                right = false;
                left = true;
            } else {
                shape.move(3, 0);  
            }
        } else if(left) {
            if (shape.getPosition().x <= 0) {
                left = false;
                right = true;
            } else {
                shape.move(-3, 0);
            }
            
        }
       
        window.clear();
        float seconds = elapsed.asSeconds();
        if (seconds >= 1) {
           clock.restart();
           frame = 0;
        } else {
           frame++; 
        }
        
        text->setString(std::to_string(frame));
        time->setString(std::to_string(seconds));
        
        window.draw(*time);
        window.draw(*text);
        window.draw(*additional);
        window.draw(*mousePosition);
        
        window.draw(shape);
        window.display();
        
    }

    return 0;
}