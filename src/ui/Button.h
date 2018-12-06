/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Button.h
 * Author: alexandr
 *
 * Created on December 6, 2018, 8:42 PM
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include "../CObject.h"

class Button: public CObject 
{
public:
    Button();
    Button(int x, int y, int width, int height);
    virtual ~Button();
    sf::Drawable* getDrawForm() override;
private:
    sf::RectangleShape* shape;
};

#endif /* BUTTON_H */

