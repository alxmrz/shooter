#ifndef APPLICATION_H
#define APPLICATION_H

#include <SFML/Graphics.hpp>
#include <string>

class Application
{
public:
    Application();
    ~Application();
    int run();
private:
    sf::Text* makeText(std::string str, int x, int );
};


#endif /* APPLICATION_H */

