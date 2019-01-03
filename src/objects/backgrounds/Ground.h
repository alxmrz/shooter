#ifndef GROUND_H
#define GROUND_H

#include "../../CObject.h"

class Ground : public CObject {
public:
    Ground();
    Ground(const Ground& orig);
    Ground(int x, int y, int width, int height);
    virtual ~Ground();
private:

};

#endif /* GROUND_H */

