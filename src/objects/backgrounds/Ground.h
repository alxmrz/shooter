#ifndef GROUND_H
#define GROUND_H

#include "../../CObject.h"

class Ground : public CObject {
public:
    Ground();
    Ground(const Ground& orig);
    Ground(int x, int y, int width, int height);
    virtual ~Ground();
    
    void update(Event event) override;
    void draw(Window* window, float dt) override;
    sf::Drawable* getDrawForm() override;
private:
    sf::RectangleShape* rect;
};

#endif /* GROUND_H */

