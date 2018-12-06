#include <SFML/Graphics.hpp>
#include "Text.h"

Text::Text() {
}

Text::Text(const Text& orig) {
}

Text::~Text() {
}

sf::Text* Text::makeText(std::string str, int x, int y)
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