#include <vector>
#include <string>
#include <iostream>
#include "GameObjects.h"
#include "CObject.h"
#include "objects/Shooter.h"
#include "ui/Button.h"

GameObjects::GameObjects()
{
}

GameObjects::~GameObjects()
{
}

std::vector<CObject*>* GameObjects::all()
{
    // TODO: fix that
    
    std::vector<CObject*>* all;
    
    /*std::cout << buttons.size() << "###" << std::endl;
    for (unsigned i = 0; i < buttons.size(); i++) {
        Button* b = (Button*)buttons[i];
        (*all)[i] = b;
        std::cout << buttons.size() << "###" << i;
    }*/
    return all;
}

void GameObjects::reset()
{
    buttons.clear();
    playable.clear();
    background.clear();
    bullets.clear();
    player = NULL;
}