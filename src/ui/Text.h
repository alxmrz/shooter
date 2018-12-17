#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <SFML/Graphics.hpp>

class Text {
public:
    Text();
    Text(std::string text, sf::Font* font, int x, int y, int fontSize = 15, sf::Color = sf::Color::Black);
    virtual ~Text();
    
    /**
     * sf::Drawable is required by Window 
     * to display simple SFML object on the screen
     * 
     * @return shape to draw 
     */
    sf::Drawable* getDrawForm();
    
    /**
     * @var current text object
     */
    sf::Text* message;
private:

};

#endif /* TEXT_H */

