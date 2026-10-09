#ifndef TREE_H
#define TREE_H

#include <vector>
#include "../../CObject.h"

class Platform;

class Tree: public CObject {
public:
    Tree();
    Tree(const Tree& orig);
    Tree(int x, int y, int trunkHeight, int numBranches, int minBranchLength, int maxBranchLength);
    virtual ~Tree();
    
    void update(Event event) override;
    void draw(Window* window, float dt) override;
    
    // Get all branches as platforms (for collision)
    std::vector<Platform*> getBranches() const;
    
private:
    std::vector<Platform*> branches;
    Platform* trunk;
    Platform* leaves;
    
    void generateBranches(int x, int y, int trunkHeight, int numBranches, int minBranchLength, int maxBranchLength);
};

#endif /* TREE_H */
