#include "Crystal.h"
#include "../../CObject.h"

Crystal::Crystal()
{    
}

Crystal::Crystal(const Crystal& orig)
{
}

Crystal::Crystal(int x, int y, int width, int height) : CObject(x, y, width, height)
{
}

Crystal::~Crystal()
{
}

