#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <SFML/Graphics.hpp>

class Text {
public:
    Text();
    Text(std::string text, int x, int y, int fontSize = 20, sf::Color = sf::Color::Black);
    virtual ~Text();

    /**
     * Creates instance of sf::Text, contained current text message
     * 
     * @param str current text
     * @param x
     * @param y
     * 
     * @return sf::Text* pointer to text object 
     */
    sf::Text* makeText(std::string str, int x, int y);
    
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

