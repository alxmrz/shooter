#include "GroundBlock.h"

GroundBlock::GroundBlock()
    : Platform(0, 0, 0, 0, sf::Color(0, 0, 100), sf::Color::Black)
{
}

GroundBlock::GroundBlock(const GroundBlock& orig)
    : Platform(0, 0, 0, 0, sf::Color(0, 0, 100), sf::Color::Black)
{
}

GroundBlock::GroundBlock(int x, int y, int width, int height)
    : Platform(x, y, width, height, sf::Color(0, 0, 100), sf::Color::Black)
{
}

GroundBlock::~GroundBlock()
{
}

void GroundBlock::update(Event event)
{
    // GroundBlock is static
}
