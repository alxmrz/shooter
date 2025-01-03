//
// Created by alexandr on 03.01.25.
//

#ifndef EVENT_H
#define EVENT_H
#include <SFML/Window/Event.hpp>

class Event {
public:
    enum Type {
        Closed,
        KeyPressed,
        KeyReleased,
        MouseWheelScrolled,
        MouseButtonPressed,
        MouseMoved
    };

    Event();

    Event(int type);

    virtual ~Event();

    bool HasType(Type type) const;

    void SetType(Type type, const sf::Event &event);

    bool IsKeyBoardUp();

    bool IsKeyBoardLeft();

    bool IsKeyBoardRight();

    bool IsKeyReleased();

private:
    int type;
    sf::Event event;
};

#endif //EVENT_H
