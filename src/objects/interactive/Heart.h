#ifndef HEART_H
#define HEART_H

#include "../../CObject.h"

class Heart: public CObject {
public:
    Heart();
    Heart(const Heart& orig);
    Heart(int x, int y, int width, int height);
    virtual ~Heart();
private:

};

#endif /* HEART_H */

