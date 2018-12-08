#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <SFML/Graphics.hpp>
//#include "Window.h"

class Text {
public:
    Text();
    Text(std::string text, int x, int y, int fontSize = 20, sf::Color = sf::Color::Black);
    virtual ~Text();
    
    sf::Text* makeText(std::string str, int x, int y);
    sf::Drawable* getDrawForm();
    //void draw(Window*) override;
    sf::Text* message;
private:
    
};

#endif /* TEXT_H */

