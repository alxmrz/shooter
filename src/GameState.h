#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class Application;
class CObject;
class GameObjects;

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
    GameObjects* objects;

    GameState();
    GameState(Application* app);
    virtual ~GameState();
    
    /**
     * Update current game state every frame
     */
    void update();
    bool isGameStarted = false;
    void playBackgroundSound();
private:
    /**
     * @var current application instance
     */
    Application* app;
    
    /**
     * @var Done for rule the gravity in the game. Not used yet.
     * TODO: need to make real gravity simulation
     */
    float gravityPower = 0.1f;
    
    float elapsedTime = 0.0;
    float velocity = 0.f;
    float acceleration = 0.5f;
    
    /**
     * Current realization of gravity, that makes player falls down. Not good one
     */
    void gravity();
    sf::Sound* backgroundLoop;

};

#endif