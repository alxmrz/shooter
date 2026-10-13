#ifndef TREE_H
#define TREE_H

#include <vector>
#include "../../CObject.h"

class Platform;

class Tree: public CObject {
public:
    static constexpr int TRUNK_WIDTH = 25;
    static constexpr int BRANCH_HEIGHT = 25;
    static constexpr int TILE_WIDTH = 50;
    static constexpr int LEAVES_WIDTH_MULTIPLIER = 7;
    static constexpr int LEAVES_X_OFFSET_MULTIPLIER = 3;

    Tree();
    Tree(const Tree& orig);
    Tree(int x, int y, int trunkHeight, int numBranches, int minBranchLength, int maxBranchLength);
    Tree(int x, int y, int trunkHeight, int numBranches, int minBranchLength, int maxBranchLength,
         int branchStep);
    Tree(int x, int y, int trunkHeight, int numBranches, int minBranchLength, int maxBranchLength,
         int branchStep, int topBranchSide, int topBranchLength = 0);
    virtual ~Tree();
    
    void update(Event event) override;
    void draw(Window* window, float dt) override;
    
    // Get all branches as platforms (for collision)
    std::vector<Platform*> getBranches() const;
    Platform* getTrunk() const;
    Platform* getLeaves() const;
    
private:
    std::vector<Platform*> branches;
    Platform* trunk;
    Platform* leaves;
    
    void drawPart(Window* window, Platform* part, float dt);

    void updateBounds();
    void includeBranches(float& minX, float& minY, float& maxX, float& maxY);
    void includeLeaves(float& minX, float& minY, float& maxX, float& maxY);
    void includePart(Platform* part, float& minX, float& minY, float& maxX, float& maxY);

    void generateBranches(int x, int y, int trunkHeight, int numBranches, int minBranchLength, int maxBranchLength);
    Platform* makeBranch(int x, int y, int trunkHeight, int minBranchLength, int maxBranchLength, int side);
    int branchXForSide(int baseX, int branchWidth, int side);

    void generateLadderBranches(int x, int trunkTop, int trunkHeight, int branchStep,
                                int minBranchLength, int maxBranchLength, int topBranchSide,
                                int topBranchLength);
    Platform* makeLadderBranch(int x, int baseY, int trunkHeight, int height, int index,
                               int minBranchLength, int maxBranchLength, int topBranchSide,
                               int topBranchLength);
    bool isStretchedTop(int height, int trunkHeight, int topBranchLength);
    Platform* makeStretchedTopBranch(int x, int branchY, int side, int topBranchLength);
    Platform* makeRandomBranch(int x, int branchY, int minBranchLength, int maxBranchLength, int side);
};

#endif /* TREE_H */
