#include "Ammunition.h"
#include "../../CObject.h"

Ammunition::Ammunition() {
}

Ammunition::Ammunition(const Ammunition& orig) {
}

Ammunition::Ammunition(int x, int y, int width, int height) : CObject(x, y, width, height)
{
}

Ammunition::~Ammunition() {
}
