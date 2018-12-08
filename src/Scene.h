#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "Application.h"
#include "GameState.h"

class Application;
class GameState;

class Scene {
public:
    Scene();
    Scene(Application* app, GameState* gameState);
    virtual ~Scene();
    void init();
    void initMainMenu();
    void initNewGame();
private:
    Application* app;
    GameState* gameState;
};

#endif /* SCENE_H */

