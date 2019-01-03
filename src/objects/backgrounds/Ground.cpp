#include <iostream>
#include "Ground.h"

Ground::Ground()
{
}

Ground::Ground(const Ground& orig)
{
}

Ground::Ground(int x, int y, int width, int height) : CObject(x, y, width, height)
{
}


Ground::~Ground()
{
}