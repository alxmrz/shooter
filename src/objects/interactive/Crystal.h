#ifndef CRYSTAL_H
#define CRYSTAL_H

#include "../../CObject.h"

class Crystal: public CObject {
public:
    Crystal();
    Crystal(const Crystal& orig);
    Crystal(int x, int y, int width, int height);
    virtual ~Crystal();
private:

};

#endif /* CRYSTAL_H */

