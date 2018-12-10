#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "GameObjects.h"
#include <vector>

class Application;
class CObject;
class GameObjects;

/*struct GameObjects 
{
    GameObjects();
    
    std::vector<CObject*> buttons;
    std::vector<CObject*> background;
    std::vector<CObject*> playable;
    Shooter* player;
    
    virtual ~GameObjects();
    std::vector<CObject*>* all();
    void reset();
};*/

class GameState {
public:
    /**
     * @var Object for getting elapsed time after last frame
     */
    sf::Clock clock;
    
    /**
     * @var Object for ruling game camera.
     */
    sf::View* view;
    
    /**
     * @var game objects container
     */
    GameObjects objects;

    GameState();
    GameState(Application* app);
    virtual ~GameState();
    
    /**
     * Update current game state every frame
     */
    void update();
private:
    /**
     * @var current application instance
     */
    Application* app;
    
    /**
     * @var Done for rule the gravity in the game. Not used yet.
     * TODO: need to make real gravity simulation
     */
    int gravityPower = 0;
    
    /**
     * Current realization of gravity, that makes player falls down. Not good one
     */
    void gravity();

};

#endif