#ifndef FLAG_H
#define FLAG_H

#include "../../CObject.h"

/**
 * The level goal: a flag placed on the highest platform. The player captures
 * it by touching it, which completes the current stage.
 */
class Flag: public CObject {
public:
    Flag();
    Flag(const Flag& orig);
    Flag(int x, int y, int width, int height);
    virtual ~Flag();
private:

};

#endif /* FLAG_H */
