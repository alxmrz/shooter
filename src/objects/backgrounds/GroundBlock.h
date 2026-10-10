#ifndef GROUNDBLOCK_H
#define GROUNDBLOCK_H

#include "../interactive/Platform.h"

class GroundBlock : public Platform {
public:
    GroundBlock();
    GroundBlock(const GroundBlock& orig);
    GroundBlock(int x, int y, int width, int height);
    virtual ~GroundBlock();

    void update(Event event) override;
};

#endif /* GROUNDBLOCK_H */
