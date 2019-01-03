#include "Heart.h"
#include "../../CObject.h"

Heart::Heart() {
}

Heart::Heart(const Heart& orig) {
}

Heart::Heart(int x, int y, int width, int height) : CObject(x, y, width, height)
{
}

Heart::~Heart() {
}