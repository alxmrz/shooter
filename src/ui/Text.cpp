#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Text.h"
#include "../Window.h"

Text::Text()
{
}

Text::Text(std::string text, sf::Font* font, int x, int y, int fontSize, sf::Color)
{
    message = new sf::Text;
    message->setFont(*font);
    message->setString(text);
    message->setCharacterSize(fontSize);
    message->setFillColor(sf::Color::Black);
    message->setPosition(x, y);
    message->setStyle(sf::Text::Bold | sf::Text::Underlined);
}

Text::~Text()
{
}

sf::Drawable* Text::getDrawForm()
{
    return message;
}