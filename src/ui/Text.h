#ifndef TEXT_H
#define TEXT_H

#include <SFML/Graphics.hpp>

class Text {
public:
    Text();
    Text(const Text& orig);
    virtual ~Text();
    
    sf::Text* makeText(std::string str, int x, int y);
private:

};

#endif /* TEXT_H */

