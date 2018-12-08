/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Shooter.h
 * Author: alexandr
 *
 * Created on December 8, 2018, 10:01 AM
 */

#ifndef SHOOTER_H
#define SHOOTER_H

#include "../CObject.h"

class Shooter: public CObject {
public:
    Shooter();
    Shooter(int x, int y, int width, int height);
    Shooter(const Shooter& orig);
    virtual ~Shooter();
    
    sf::Sprite* sprite;
    sf::Texture* texture;
    sf::Drawable* getDrawForm() override;
    void move(int x, int y);
    void setFormColor(sf::Color);
private:

};

#endif /* SHOOTER_H */

