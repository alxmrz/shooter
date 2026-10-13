#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Event.h"

class Application;
class CObject;
class GameObjects;
class AI;

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
    void update(Event event);
    bool isGameStarted = false;
    bool isGamePaused = false;
    void playBackgroundMusic();
    void stopBackgroundMusic();
    void startNewGame();
    float getElapsedTime();
    void resetElapsedTime();
private:
    /* @var current application instance */
    Application* app;
    
    sf::Sound* backgroundMusic;
    
    /* Elapsed time from list frame*/
    float elapsedTime = 0.0;

    bool isGameplayRunning();
    void updateRunningGame(Event event);
    void updateDialogue(Event event);
    void updateGameplay(Event event);
    void updateWorld(Event event);
    void updatePlayerDeath();
    bool isDeathAnimationFinished();
    bool isPlayerDeathPending();
    bool hasDeadPlayer();
};

#endif