#include <vector>
#include <string>
#include <iostream>
#include "GameState.h"
#include "ui/Button.h"
#include "CObject.h"


GameObjects::GameObjects()
{
    
}

GameObjects::~GameObjects()
{
   
}
std::vector<CObject*>*  GameObjects::all()
{
    std::vector<int>* all;
    std::vector<CObject*>* al1l;
    
    std::cout << buttons.size() << "###" << std::endl;
    for (unsigned i = 0; i < buttons.size(); i++) {
        Button* b = (Button*)buttons[i];
        (*al1l)[i] = b;
        std::cout << buttons.size() << "###" << i;
    }
    return al1l;
}

void GameObjects::reset()
{
    buttons.clear();
    playable.clear();
    player = NULL;
}