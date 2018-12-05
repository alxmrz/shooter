#pragma once
#ifndef SCENE_H
#define SCENE_H


#include "GameState.h"
#include "Application.h"

class Application;
class GameState;
class Scene {
public:
    Scene();
    Scene(Application* app, GameState* gameState);
    virtual ~Scene();
    void init();
private:
    Application* app;
    GameState* gameState;
};

#endif /* SCENE_H */

