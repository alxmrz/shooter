#ifndef EVENT_H
#define EVENT_H

#include "GameState.h"
#include "Application.h"


class Event {
public:
    Event();
    Event(Application* app, GameState* gameState);
    virtual ~Event();
    void handle();
private:
    Application* app;
    GameState* gameState;
};

#endif /* EVENT_H */

