#include "Flag.h"
#include "../../CObject.h"

Flag::Flag()
{
}

Flag::Flag(const Flag& orig)
{
}

Flag::Flag(int x, int y, int width, int height) : CObject(x, y, width, height)
{
}

Flag::~Flag()
{
}
