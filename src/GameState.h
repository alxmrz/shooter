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
    bool isGamePaused = false;
    void playBackgroundSound();
    void stopBackgroundSound();
    void startNewGame();
private:
    /**
     * @var current application instance
     */
    Application* app;
    
    sf::Sound* backgroundLoop;
    float elapsedTime = 0.0;
    void incrementDelayedTime();
    /**
     * Current realization of gravity, that makes player falls down. Not good one
     */
    void causeGravity();
    void updateBulletsState();
};

#endif