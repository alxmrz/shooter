#ifndef AMMUNITION_H
#define AMMUNITION_H

#include "../../CObject.h"

class Ammunition: public CObject {
public:
    Ammunition();
    Ammunition(const Ammunition& orig);
    Ammunition(int x, int y, int width, int height);
    virtual ~Ammunition();
private:

};

#endif /* AMMUNITION_H */

