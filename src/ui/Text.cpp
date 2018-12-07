#include <string>
#include <SFML/Graphics.hpp>
#include "Text.h"
#include "../Window.h"


Text::Text() {
}

Text::Text(std::string text, int x, int y, int fontSize, sf::Color) {
    this->message = makeText(text, x, y);
}

Text::~Text() {
}


sf::Drawable* Text::getDrawForm()
{
    return message;
}

sf::Text* Text::makeText(std::string str, int x, int y)
{
    sf::Font* font = new sf::Font;
    if(!font->loadFromFile("fonts/arial.ttf")) {
//        std::cout << "No fonts";
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