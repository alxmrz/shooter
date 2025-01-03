//
// Created by alexandr on 03.01.25.
//
#include "Event.h"

#include <utility>
#include <SFML/Window/Event.hpp>


Event::Event()
= default;

Event::Event(int type):
type(type)
{

}

Event::~Event() = default;

bool Event::HasType(const Type type) const {
    return this->type == type;
}

void Event::SetType(const Type type, const sf::Event &event) {
    this->type = type;
    this->event = event;
}

bool Event::IsKeyBoardUp() {
    return this->event.key.code == sf::Keyboard::Up;
}

bool Event::IsKeyBoardLeft() {
    return this->event.key.code == sf::Keyboard::Left;
}

bool Event::IsKeyBoardRight() {
    return this->event.key.code == sf::Keyboard::Right;
}

bool Event::IsKeyReleased() {
    return this->event.type == sf::Event::KeyReleased;
}
